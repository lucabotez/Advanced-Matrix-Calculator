// Copyright Croitoru Constantin-Bogdan Grupa 314CA 2022-2023
// Copyright Botez Luca Grupa 314CA 2022-2023
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functii.h"
#include "functii_2.h"
#include "list.h"
#include "rotatie_matrice.h"
#include "strassen.h"

#define DIE(assertion, call_description)  \
	do {                                  \
		if (assertion) {                  \
			fprintf(stderr, "(%s, %d): ", \
					__FILE__, __LINE__);  \
			perror(call_description);     \
			exit(errno);                  \
		}                                 \
	} while (0)

#define MAX_STRING_SIZE 64

void eliberare_memorie_matrice(int **mat, int nr_l);

#define D_MOD 10007

// se incarca in memorie o matrice
void incarcare_matrice(linked_list_t *list) {
	int nr_l, nr_c, i, j;
	scanf("%d", &nr_l);
	scanf("%d", &nr_c);

	int **mat;
	mat = malloc(nr_l * sizeof(int *));
	for (int i = 0; i < nr_l; i++)
		mat[i] = malloc(nr_c * sizeof(int));

	for (i = 0; i < nr_l; i++)
		for (j = 0; j < nr_c; j++)
			scanf("%d", &mat[i][j]);

	ll_add_nth_node(list, list->size, mat, nr_l, nr_c);

	// eliberare_memorie_matrice(mat,nr_l);
	for (int i = 0; i < nr_l; i++)
		free(mat[i]);
	free(mat);
}

// se afiseaza din vectorul de dimesiuni, dimensiunea matricei cu indicele cit
void dimensiune(linked_list_t *list) {
	int indice;
	scanf("%d", &indice);
	if (indice >= list->size || indice < 0)
		printf("No matrix with the given index\n");
	else {
		ll_node_t *node = ll_get_nth_node(list, indice);
		printf("%d %d\n", node->linie, node->coloana);
	}
}

// se afiseaza elementele matricei cu indicile citit
void afisare(linked_list_t *list) {
	int i, j, indice;
	scanf("%d", &indice);

	if (indice >= list->size || indice < 0) {
		printf("No matrix with the given index\n");
	} else {
		// nr_l numarul de linii, nr_c numarul de coloane

		ll_node_t *node = ll_get_nth_node(list, indice);
		int nr_l = node->linie, nr_c = node->coloana;
		int **mat = (int **)node->data;
		for (i = 0; i < nr_l; i++) {
			for (j = 0; j < nr_c; j++)
				printf("%d ", mat[i][j]);
			printf("\n");
		}
	}
}

// redimensioneaza matricea cu indicele citit
void redimensionare(linked_list_t *list) {
	int indice, nr_l, nr_c, j, i;  // nr_l numar de linii, nr_c numar de coloane
	scanf("%d", &indice);
	scanf("%d", &nr_l);

	int *l_indici = (int *)malloc(nr_l * sizeof(int));  // vec care retine linii
	if (!l_indici) {
		fprintf(stderr, "Nu s-a putut aloca");
	}
	for (i = 0; i < nr_l; i++)
		scanf("%d", &l_indici[i]);

	scanf("%d", &nr_c);
	int *c_indici = (int *)malloc(nr_c * sizeof(int));  // vec care retine col
	if (!c_indici) {
		fprintf(stderr, "Nu s-a putut aloca");
	}
	for (i = 0; i < nr_c; i++)
		scanf("%d", &c_indici[i]);

	if (indice >= list->size || indice < 0) {
		printf("No matrix with the given index\n");
	} else {
		int **aux;  // matrice auxiliara
		aux = (int **)malloc(nr_l * sizeof(int *));
		if (!aux) {
			fprintf(stderr, "Nu s-a putut aloca");
		} else {
			for (i = 0; i < nr_l; i++) {
				aux[i] = (int *)malloc(nr_c * sizeof(int));
				if (!aux[i]) {
					fprintf(stderr, "Nu s-a putut aloca");
				}
			}
			ll_node_t *node = ll_remove_nth_node(list, indice);
			for (i = 0; i < nr_l; i++)
				for (j = 0; j < nr_c; j++)
					aux[i][j] = node->data[l_indici[i]][c_indici[j]];

			for (int i = 0; i < node->linie; i++)
				free(node->data[i]);
			free(node->data);
			free(node);

			ll_add_nth_node(list, indice, aux, nr_l, nr_c);

			for (i = 0; i < nr_l; i++)
				free(aux[i]);
			free(aux);
		}
	}
	free(l_indici);
	free(c_indici);
}

// se realizea inmultirea a doua matrice
void inmultire(linked_list_t *list) {
	int indice1, indice2;

	scanf("%d%d", &indice1, &indice2);
	if (indice1 >= list->size || indice1 < 0 || indice2 >= list->size || indice2 < 0) {
		printf("No matrix with the given index\n");
	} else {
		ll_node_t *nod1, *nod2;
		nod1 = ll_get_nth_node(list, indice1);
		nod2 = ll_get_nth_node(list, indice2);
		if (nod1->coloana != nod2->linie)
			printf("Cannot perform matrix multiplication\n");
		else {
			int **mat;
			mat = malloc(nod1->linie * sizeof(int *));
			for (int i = 0; i < nod1->linie; i++)
				mat[i] = malloc(nod2->coloana * sizeof(int));

			// se face inmultirea
			int i, j, k;
			for (i = 0; i < nod1->linie; i++)
				for (k = 0; k < nod2->coloana; k++) {
					int a = 0;
					for (j = 0; j < nod1->coloana; j++) {
						int aux = nod1->data[i][j];
						a = (a + aux * nod2->data[j][k]) % D_MOD;
					}
					a = a % D_MOD;
					if (a < 0)
						a += D_MOD;
					mat[i][k] = a;
				}

			ll_add_nth_node(list, list->size, mat, nod1->linie, nod2->coloana);

			for (int i = 0; i < nod1->linie; i++)
				free(mat[i]);
			free(mat);
			// eliberare_memorie_matrice(mat,nod1->linie);
		}
	}
}

// se sorteaza vectorul de matrice in functie de suma elem fiecarei matrice
void sortare(linked_list_t *list) {
	int i, j, s = 0, k;
	int *a;
	ll_node_t *node;

	// a=vector care retine suma pentru fiecare matrice
	a = (int *)malloc(list->size * sizeof(int));

	// se calculeaza suma pentru fiecare matrice si e retinuta in a
	for (i = 0; i < list->size; i++) {
		s = 0;
		node = ll_get_nth_node(list, i);
		for (j = 0; j < node->linie; j++)
			for (k = 0; k < node->coloana; k++)
				s = (s + node->data[j][k]) % D_MOD;
		s = s % D_MOD;
		if (s < 0)
			s = s + D_MOD;
		a[i] = s;
	}

	ll_node_t *node1, *node2;
	// se ordoneazam vectorul de sume a
	for (i = 0; i < list->size - 1; i++)
		for (j = i + 1; j < list->size; j++)
			if (a[i] > a[j]) {
				int aux1 = a[i];
				a[i] = a[j];
				a[j] = aux1;

				// nu stiu daca merge

				node2 = ll_get_nth_node(list, j);
				node1 = ll_remove_nth_node(list, i);
				ll_add_nth_node(list, i, node2->data, node2->linie, node2->coloana);

				// trebuie eliberata memorie pentru node2

				node2 = ll_remove_nth_node(list, j);
				eliberare_memorie_matrice(node2->data, node2->linie);
				free(node2->data);
				free(node2);

				ll_add_nth_node(list, j, node1->data, node1->linie, node1->coloana);

				eliberare_memorie_matrice(node1->data, node1->linie);
				free(node1->data);
				free(node1);
			}

	free(a);
}

// se realizeaza transpunerea unei matrice
void transpunere(linked_list_t *list) {
	int indice, i, j;

	scanf("%d", &indice);
	if (indice >= list->size || indice < 0) {
		printf("No matrix with the given index\n");
	} else {
		int **a;  // matrice auxiliara care retine elemetele matricei
		ll_node_t *node = ll_remove_nth_node(list, indice);
		a = malloc(node->coloana * sizeof(int *));
		for (int i = 0; i < node->coloana; i++)
			a[i] = malloc(node->linie * sizeof(int));

		// se copiaza in matrice elementele, dar cu indicile schimbat
		for (i = 0; i < node->coloana; i++)
			for (j = 0; j < node->linie; j++)
				a[i][j] = node->data[j][i];

		ll_add_nth_node(list, indice, a, node->coloana, node->linie);

		// tebuie eliberata memoria pentru nodul eliminat !!!
		for (int i = 0; i < node->coloana; i++)
			free(a[i]);
		free(a);

		eliberare_memorie_matrice(node->data, node->linie);
		free(node->data);
		free(node);
	}
}

// functie folosita pentru inmultirea a doua matrici de la ridicarea la putere
// retine rezultatul in prima matrice din antet

void inmultire_mat_R(int **a, int **b, int dim) {
	int **mat_rez;  // retine rezultatul inmultirii
	mat_rez = (int **)malloc(dim * sizeof(int *));
	if (!mat_rez) {
		fprintf(stderr, "Nu s-a putut aloca");
		exit(-1);
	} else {
		for (int i = 0; i < dim; i++) {
			mat_rez[i] = (int *)malloc(dim * sizeof(int));
			if (!mat_rez[i]) {
				fprintf(stderr, "Nu s-a putut aloca");
				for (int j = 0; j <= i; j++)
					free(mat_rez[j]);
				exit(-1);
			}
		}

		// se realizeaza inmultirea
		for (int i = 0; i < dim; i++)
			for (int k = 0; k < dim; k++) {
				int c = 0;
				for (int j = 0; j < dim; j++)
					c = (c + a[i][j] * b[j][k]) % D_MOD;
				c = c % D_MOD;
				if (c < 0)
					c += D_MOD;
				mat_rez[i][k] = c;
			}

		// se copiaza rezultatul in prima matrice din antet
		for (int i = 0; i < dim; i++)
			for (int j = 0; j < dim; j++)
				a[i][j] = mat_rez[i][j];

		// se elibereaza memoria pentru matricea mat_rez
		for (int i = 0; i < dim; i++)
			free(mat_rez[i]);
		free(mat_rez);
	}
}

// se ridica o matrice la o putere in timp logaritmic
void ridicare(linked_list_t *list) {
	int index, putere;
	scanf("%d%d", &index, &putere);
	if (index >= list->size || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		if (putere < 0) {
			printf("Power should be positive\n");
		} else {
			ll_node_t *node = ll_get_nth_node(list, index);
			if (node->linie != node->coloana)
				printf("Cannot perform matrix multiplication\n");
			else {
				// matricea ce retine rezutatul inmultirii in cazul in care puterea e impara
				// initial aceasta e In( elementul neutru)

				int **b, i, j;
				b = (int **)malloc(node->linie * sizeof(int *));
				for (int i = 0; i < node->linie; i++)
					b[i] = malloc(node->linie * (sizeof(int)));

				for (i = 0; i < node->linie; i++)
					for (j = 0; j < node->linie; j++)
						if (i == j)
							b[i][i] = 1;
						else
							b[i][j] = 0;

				if (putere == 0) {
					for (i = 0; i < node->linie; i++)
						for (j = 0; j < node->linie; j++)
							if (i == j)
								node->data[i][j] = 1;
							else
								node->data[i][j] = 0;
				} else {
					int aux = node->linie;
					while (putere > 1) {
						if (putere % 2 == 1)
							inmultire_mat_R(b, node->data, node->linie);

						inmultire_mat_R(node->data, node->data, aux);
						putere = putere / 2;
					}

					// rezultatul e retinut direct in matrice
					inmultire_mat_R(node->data, b, aux);

					// trebuie sa modific eliberarea dememorie pentru b
					for (i = 0; i < node->linie; i++)
						free(b[i]);
					free(b);
				}
			}
		}
	}
}
void eliberare_memorie_matrice(int **mat, int nr_l) {
	for (int i = 0; i < nr_l; i++)
		free(mat[i]);
	// free(mat);
}
// se elibereaza memoria pentru o matrice
void eliberare_mat(linked_list_t *list) {
	int indice;

	scanf("%d", &indice);
	if (indice >= list->size || indice < 0) {
		printf("No matrix with the given index\n");
	} else {
		ll_node_t *node = ll_remove_nth_node(list, indice);
		eliberare_memorie_matrice(node->data, node->linie);
		free(node->data);
		free(node);
	}
}

// se elibereaza toata memoria
void eliberare(linked_list_t *list) {
	int i;
	if (list->size) {
		int nr = list->size;
		for (i = 0; i < nr; i++) {
			ll_node_t *node = ll_remove_nth_node(list, 0);
			eliberare_memorie_matrice(node->data, node->linie);
			free(node->data);
			free(node);
		}
	}
}

int main(void) {
	linked_list_t *list;
	list = ll_create(sizeof(int));
	char comanda[100];
	int stop = 1;
	while (stop) {
		fscanf(stdin, "%s", comanda);
		if (strcmp(comanda, "L") == 0) {
			incarcare_matrice(list);

		} else if (strcmp(comanda, "inverse") == 0) {
			inverse_matrix(list);
		} else if (strcmp(comanda, "subtract") == 0) {
			subtract(list);
		} else if (strcmp(comanda, "min") == 0) {
			min_elem(list);
		} else if (strcmp(comanda, "trace") == 0) {
			trace(list);
		} else if (strcmp(comanda, "hadamard") == 0) {
			hadamard(list);
		} else if (strcmp(comanda, "upper") == 0) {
			upper(list);
		} else if (strcmp(comanda, "eig") == 0) {
			eig(list);

		} else if (strcmp(comanda, "determinant") == 0) {
			int indice;
			scanf("%d", &indice);
			ll_node_t *node = ll_get_nth_node(list, indice);
			int determinant;
			determinant = aflare_determinant(node->data, node->linie);
			printf("Determinantul matricei este %d\n", determinant);

		} else if (strcmp(comanda, "ridicare") == 0) {
			int indice, k;
			scanf("%d", &indice);
			scanf("%d", &k);
			ll_node_t *node = ll_get_nth_node(list, indice);
			ridicare_fiecare_elem(node, node->linie, node->coloana, k);

		} else if (strcmp(comanda, "maxim") == 0) {
			int indice;
			scanf("%d", &indice);
			ll_node_t *node = ll_get_nth_node(list, indice);
			int maxi = determinare_maxim(node->data, node->linie, node->coloana);
			printf("Maximul este %d\n", maxi);

		} else if (strcmp(comanda, "rang") == 0) {
			int indice;
			scanf("%d", &indice);
			ll_node_t *node = ll_get_nth_node(list, indice);
			int maxi = determinare_rang(node->data, node->linie, node->coloana);
			printf("Rangul este %d\n", maxi);

		} else if (strcmp(comanda, "doolittle") == 0) {
			int indice;
			scanf("%d", &indice);
			ll_node_t *node = ll_get_nth_node(list, indice);

			double **mat;
			mat = alocare_mat_double(node->linie, node->linie);
			for (int i = 0; i < node->linie; i++)
				for (int j = 0; j < node->coloana; j++)
					mat[i][j] = (double)node->data[i][j];

			doolittle_algoritm(mat, node->linie);

		} else if (strcmp(comanda, "rotatie") == 0) {
			rotatie(list);

		} else if (strcmp(comanda, "strassen") == 0) {
			strassen(list);

		} else if (strcmp(comanda, "D") == 0) {
			dimensiune(list);
		} else if (strcmp(comanda, "P") == 0) {
			afisare(list);
		} else if (strcmp(comanda, "C") == 0) {
			redimensionare(list);
		} else if (strcmp(comanda, "M") == 0) {
			inmultire(list);
		} else if (strcmp(comanda, "O") == 0) {
			sortare(list);
		} else if (strcmp(comanda, "T") == 0) {
			transpunere(list);
		} else if (strcmp(comanda, "R") == 0) {
			ridicare(list);
		} else if (strcmp(comanda, "F") == 0) {
			eliberare_mat(list);
		} else if (strcmp(comanda, "S") == 0) {
			strassen(list);
		} else if (strcmp(comanda, "Q") == 0) {
			eliberare(list);
			stop = 0;
		} else {
			printf("Unrecognized command\n");
		}
	}

	free(list);

	return 0;
}
