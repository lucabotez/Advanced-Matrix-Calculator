// Croitoru Bogdan
#ifndef STRASSEN_H_
#define STRASSEN_H_

#include "list.h"

void strassen_c(linked_list_t *list);
void strassen(linked_list_t *list);
int **strassen_inmultire(int **mat1, int **mat2, int n);
int **mat_adunare(int **a, int **b, int n);
int **mat_scadere(int **a, int **b, int n);

#endif /* STRASSEN_H_ */
