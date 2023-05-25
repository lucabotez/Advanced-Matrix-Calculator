// Copyright Croitoru Constantin-Bogdan Grupa 314CA 2022-2023
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MODULO 10007
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

typedef struct ll_node_t {
	int **data;
	int linie;
	int coloana;
	struct ll_node_t *next;
} ll_node_t;

typedef struct linked_list_t {
	ll_node_t *head;
	unsigned int data_size;
	int size;
} linked_list_t;

ll_node_t *
node_create(int **new_data, unsigned int data_size, int nr_l, int nr_c) {
	ll_node_t *new;
	new = malloc(sizeof(ll_node_t));
	DIE(new == NULL, "Nu s-a putut aloca");

	new->data = malloc(nr_l * sizeof(int *));
	for (int i = 0; i < nr_l; i++)
		new->data[i] = malloc(nr_c * data_size);

	for (int i = 0; i < nr_l; i++)
		for (int j = 0; j < nr_c; j++)

			new->data[i][j] = new_data[i][j];

	new->linie = nr_l;
	new->coloana = nr_c;

	return new;
}

linked_list_t *
ll_create(unsigned int data_size) {
	/* TODO */
	linked_list_t *list = malloc(sizeof(linked_list_t));
	DIE(list == NULL, "Nu s-a putut aloca");
	list->data_size = data_size;
	list->size = 0;
	list->head = NULL;
	return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, ignorati comanda (iesiti din functie).
 */
void ll_add_nth_node(linked_list_t *list, int n, int **new_data, int nr_l, int nr_c) {
	/* TODO */
	if (list == NULL)
		return;

	if (n >= 0) {
		ll_node_t *new;
		new = node_create(new_data, list->data_size, nr_l, nr_c);

		if (n >= list->size)
			n = list->size;

		if (list->size == 0 || n == 0) {
			new->next = list->head;
			list->head = new;
			list->size++;
		}

		else {
			ll_node_t *curr;
			curr = list->head;

			int k = 0;
			while (curr->next != NULL && k < n - 1) {
				curr = curr->next;
				k++;
			}

			new->next = curr->next;

			curr->next = new;
			list->size++;
		}
	}
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, ignorati comanda (iesiti din functie).
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
ll_node_t *
ll_remove_nth_node(linked_list_t *list, int n) {
	ll_node_t *prev, *curr;

	if (list == NULL) {
		return NULL;
	}

	if (list->head == NULL) { /* Lista este goala. */
		return NULL;
	}

	/* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
	if (n > list->size - 1) {
		n = list->size - 1;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (prev == NULL) {
		/* Adica n == 0. */
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}
/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t *list) {
	/* TODO */
	return (unsigned int)list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */

void ll_free(linked_list_t **pp_list) {
	/* TODO */
	ll_node_t *curr, *prev;
	curr = (*pp_list)->head;
	prev = NULL;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		eliberare_memorie_matrice(prev->data, prev->linie);
		free(prev);
	}
	free(*pp_list);
	*pp_list = NULL;
}

// functia returneaza al n-lea nod
ll_node_t *ll_get_nth_node(linked_list_t *list, unsigned int n) {
	if ((int)n >= list->size)
		n = n - 1;

	ll_node_t *curr = list->head;
	unsigned int nr = 0;

	// se parcurge lista pana la nodul n
	while (nr < n) {
		curr = curr->next;
		nr++;
	}

	return curr;
}

#define D_MOD 10007

// antet functie de eliberare resurse

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

int aflare_determinant(int **mat, int n) {
	int num1, num2, det = 1, index, total = 1;
	int temp[n + 1];

	// loop for traversing the diagonal elements
	for (int i = 0; i < n; i++) {
		index = i;

		// se sare peste elementele care sunt nule
		while (index < n && mat[index][i] == 0) {
			index++;
		}

		// daca exista un element diferit de 0
		if (index == n) {
			continue;
		}

		if (index != i) {
			// se interschimba valorile
			for (int j = 0; j < n; j++) {
				int aux = mat[index][j];
				mat[index][j] = mat[i][j];
				mat[i][j] = aux;
			}

			// se tine cont de semnul determinantului din formula
			det = det * pow(-1, index - i);
		}

		// stocheaza elementele
		for (int j = 0; j < n; j++) {
			temp[j] = mat[i][j];
		}
		// traversing every row below the diagonal element
		for (int j = i + 1; j < n; j++) {
			// valuarea elementului de pe diagonala
			num1 = temp[i];
			// valoarea urmatorului element de pe linie
			num2 = mat[j][i];

			for (int k = 0; k < n; k++)
				mat[j][k] = (num1 * mat[j][k]) - (num2 * temp[k]);

			total = total * num1;
		}
	}

	for (int i = 0; i < n; i++) {
		det = det * mat[i][i];
	}
	return (det / total);
}

void determinare_maxim(int **mat, int nr_l, int nr_c) {
	int maxi = mat[0][0];
	for (int i = 0; i < nr_l; i++)
		for (int j = 0; j < nr_c; j++)
			if (maxi < mat[i][j])
				maxi = mat[i][j];

	printf("Elementul maxim este %d\n", maxi);
}

void ridicare_put_2_fiecare_elem(int **mat, int nr_l, int nr_c, int k) {
	int **aux;
	aux = malloc(nr_l * sizeof(int *));
	for (int i = 0; i < nr_l; i++)
		aux[i] = calloc(nr_c, sizeof(int));

	for (int i = 0; i < nr_l; i++)
		for (int j = 0; j < nr_c; j++) {
			aux[i][j] = mat[i][j];
			for (int p = 0; p < k - 1; p++)
				aux[i][j] *= mat[i][j];
		}

	for (int i = 0; i < nr_l; i++) {
		for (int j = 0; j < nr_c; j++)
			printf("%d ", aux[i][j]);
		printf("\n");
	}

	for (int i = 0; i < nr_l; i++)
		free(aux[i]);
	free(aux);
}

int **alocare_matrice(int rows, int columns) {
	int **matrix = malloc(rows * sizeof(int *));
	if (!matrix)
		return NULL;
	for (int i = 0; i < rows; i++) {
		matrix[i] = malloc(columns * sizeof(int));
		if (!matrix[i]) {
			return NULL;
			free(matrix[i]);
		}
	}
	return matrix;
}

void free_matrice(int rows, int **matrix) {
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

int **mat_adunare(int **a, int **b, int n) {
	int **aux = alocare_matrice(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] = (a[i][j] + b[i][j]) % MODULO;

	return aux;
}

int **mat_scadere(int **a, int **b, int n) {
	int **aux = alocare_matrice(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] = (a[i][j] - b[i][j]) % MODULO;

	return aux;
}

// elibereaza memoria pentru matricile in care s-a realizat inmultirea
void free_m(int **m1, int **m2, int **m3, int **m4,
			int **m5, int **m6, int **m7, int dim) {
	free_matrice(dim, m1);
	free_matrice(dim, m2);
	free_matrice(dim, m3);
	free_matrice(dim, m4);
	free_matrice(dim, m5);
	free_matrice(dim, m6);
	free_matrice(dim, m7);
}

// elibereaza memoria pentru matricile care au retinut diferenta
void free_s(int **s1, int **s2, int **s3, int **s4, int dim) {
	free_matrice(dim, s1);
	free_matrice(dim, s2);
	free_matrice(dim, s3);
	free_matrice(dim, s4);
}

// elibereaza memoria pentru matricile care au retinut suma
void free_a(int **a1, int **a2, int **a3, int **a4,
			int **a5, int **a6, int **a7, int **a8,
			int **a9, int **a10, int dim) {
	free_matrice(dim, a1);
	free_matrice(dim, a2);
	free_matrice(dim, a3);
	free_matrice(dim, a4);
	free_matrice(dim, a5);
	free_matrice(dim, a6);
	free_matrice(dim, a7);
	free_matrice(dim, a8);
	free_matrice(dim, a9);
	free_matrice(dim, a10);
}

void free_c(int **c1, int **c2, int **c3, int **c4, int dim) {
	free_matrice(dim, c1);
	free_matrice(dim, c2);
	free_matrice(dim, c3);
	free_matrice(dim, c4);
}

void free_q(int ***q1, int ***q2, int dim) {
	free_matrice(dim, q1[0]);
	free_matrice(dim, q2[0]);
	free_matrice(dim, q1[1]);
	free_matrice(dim, q2[1]);
	free_matrice(dim, q1[2]);
	free_matrice(dim, q2[2]);
	free_matrice(dim, q1[3]);
	free_matrice(dim, q2[3]);

	free(q1);
	free(q2);
}

// acesta este functia care implementeaza algoritmul strssen
int **strassen_inmultire(int **mat1, int **mat2, int n) {
	if (n == 1) {
		int **c = alocare_matrice(1, 1);
		c[0][0] = mat1[0][0] * mat2[0][0];
		return c;
	}

	int **c = alocare_matrice(n, n);
	int dim = n / 2;
	int k = 4;

	int ***q1 = malloc(4 * sizeof(int **)), ***q2 = malloc(4 * sizeof(int **));

	// fiecare matrice va fi împărțită în 4 mai mici
	// fiecare q va fi un sfert
	for (int i = 0; i < k; i++) {
		q1[i] = alocare_matrice(dim, dim);
		q2[i] = alocare_matrice(dim, dim);
	}
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			q1[0][i][j] = mat1[i][j];
			q2[0][i][j] = mat2[i][j];
			q1[1][i][j] = mat1[i][j + dim];
			q2[1][i][j] = mat2[i][j + dim];
			q1[2][i][j] = mat1[i + dim][j];
			q2[2][i][j] = mat2[i + dim][j];
			q1[3][i][j] = mat1[i + dim][j + dim];
			q2[3][i][j] = mat2[i + dim][j + dim];
		}

	int **a1 = mat_adunare(q1[0], q1[3], dim);
	int **a2 = mat_adunare(q2[0], q2[3], dim);
	int **a3 = mat_adunare(q1[2], q1[3], dim);
	int **a4 = mat_adunare(q1[0], q1[1], dim);
	int **a5 = mat_adunare(q2[0], q2[1], dim);
	int **a6 = mat_adunare(q2[2], q2[3], dim);

	int **s1 = mat_scadere(q2[1], q2[3], dim);
	int **s2 = mat_scadere(q2[2], q2[0], dim);
	int **s3 = mat_scadere(q1[2], q1[0], dim);
	int **s4 = mat_scadere(q1[1], q1[3], dim);

	// aceste matrice recursive vor deveni mai mici
	// și mai mici până ating dimensiunea 1 x 1
	int **m1 = strassen_inmultire(a1, a2, dim);
	int **m2 = strassen_inmultire(a3, q2[0], dim);
	int **m3 = strassen_inmultire(q1[0], s1, dim);
	int **m4 = strassen_inmultire(q1[3], s2, dim);
	int **m5 = strassen_inmultire(a4, q2[3], dim);
	int **m6 = strassen_inmultire(s3, a5, dim);
	int **m7 = strassen_inmultire(s4, a6, dim);

	free_s(s1, s2, s3, s4, dim);
	free_q(q1, q2, dim);
	int **a7 = mat_adunare(m1, m4, dim);
	int **a8 = mat_adunare(a7, m7, dim);
	int **a9 = mat_adunare(m1, m3, dim);
	int **a10 = mat_adunare(a9, m6, dim);

	int **c1 = mat_scadere(a8, m5, dim);
	int **c2 = mat_adunare(m3, m5, dim);
	int **c3 = mat_adunare(m2, m4, dim);
	int **c4 = mat_scadere(a10, m2, dim);
	free_m(m1, m2, m3, m4, m5, m6, m7, dim);
	free_a(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, dim);
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			c[i][j] = c1[i][j] % MODULO;
			c[i][j + dim] = c2[i][j] % MODULO;

			if (c[i][j] < 0)
				c[i][j] = c[i][j] + MODULO;
			if (c[i][j + dim] < 0)
				c[i][j + dim] = c[i][j + dim] + MODULO;
			c[i + dim][j] = c3[i][j] % MODULO;
			if (c[i + dim][j] < 0)
				c[i + dim][j] = c[i + dim][j] + MODULO;
			c[i + dim][j + dim] = c4[i][j] % MODULO;
			if (c[i + dim][j + dim] < 0)
				c[i + dim][j + dim] = c[i + dim][j + dim] + MODULO;
		}
	free_c(c1, c2, c3, c4, dim);
	return c;
}

void strassen(linked_list_t *list, int ind1, int ind2) {
	if (ind1 > +list->size || ind1 <= -1 || ind2 >= list->size || ind2 <= -1) {
		printf("No matrix with the given index\n");
	} else {
		ll_node_t *nod1, *nod2;
		nod1 = ll_get_nth_node(list, ind1);
		nod2 = ll_get_nth_node(list, ind2);
		if (nod1->coloana == nod2->linie) {
			int **mat = strassen_inmultire(nod1->data, nod2->data, nod2->linie);

			ll_add_nth_node(list, list->size, mat, nod1->linie, nod2->coloana);

			for (int i = 0; i < nod1->linie; i++)
				free(mat[i]);
			free(mat);

		} else {
			printf("Cannot perform matrix multiplication\n");
		}
	}
}

int main(void) {
	linked_list_t *list;
	list = ll_create(sizeof(int));
	char comanda;
	int stop = 1;
	while (stop) {
		scanf("%c", &comanda);
		if (comanda == 'L') {
			incarcare_matrice(list);
		} else if (comanda == 'b') {
			int indice;
			scanf("%d", &indice);
			ll_node_t *node = ll_get_nth_node(list, indice);

			int determinant;
			determinant = aflare_determinant(node->data, node->linie);
			printf("Determinantul matricei este %d\n", determinant);
		} else if (comanda == 'r') {
			int indice, k;
			scanf("%d", &indice);
			scanf("%d", &k);
			ll_node_t *node = ll_get_nth_node(list, indice);

			ridicare_put_2_fiecare_elem(node->data, node->linie, node->coloana, k);

		} else if (comanda == 'e') {
			int indice;
			scanf("%d", &indice);
			ll_node_t *node = ll_get_nth_node(list, indice);

			determinare_maxim(node->data, node->linie, node->coloana);

		} else if (comanda == 'D') {
			dimensiune(list);
		} else if (comanda == 'P') {
			afisare(list);
		} else if (comanda == 'C') {
			redimensionare(list);
		} else if (comanda == 'M') {
			inmultire(list);
		} else if (comanda == 'O') {
			sortare(list);
		} else if (comanda == 'T') {
			transpunere(list);
		} else if (comanda == 'R') {
			ridicare(list);
		} else if (comanda == 'F') {
			eliberare_mat(list);

		} else if (comanda == 'S') {
			int indice1, indice2;
			scanf("%d", &indice1);
			scanf("%d", &indice2);

			strassen(list, indice1, indice2);

		} else if (comanda == 'Q') {
			eliberare(list);
			stop = 0;
		} else {
			printf("Unrecognized command\n");
		}

		getchar();
	}

	free(list);

	return 0;
}
