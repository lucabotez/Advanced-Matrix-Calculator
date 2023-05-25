#include "strassen.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MODULO 10007

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

// acesta este functia care implementeaza algoritmul strassen
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
	// fiecare q va reprezenta un sfert
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

void strassen(linked_list_t *list) {
	int ind1, ind2;
	scanf("%d", &ind1);
	scanf("%d", &ind2);

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
