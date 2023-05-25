// Croitoru Bogdan
#ifndef FUNCTII_H_
#define FUNCTII_H_

#include "list.h"
#define MAX 100

int **alocare_mat(int nr_l, int nr_c);
double **alocare_mat_double(int nr_l, int nr_c);

int aflare_determinant(int **mat, int n);
int determinare_rang(int **mat, int nr_l, int nr_c);
int determinare_maxim(int **mat, int nr_l, int nr_c);
void ridicare_fiecare_elem(ll_node_t *nod, int nr_l, int nr_c, int k);

void doolittle_algoritm(double **A, int n);

#endif /* FUNCTII_H_ */
