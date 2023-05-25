#ifndef FUNCTII_2_H_
#define FUNCTII_2_H_

#include "list.h"

float float_determinant(float **matrix, int dim);
float inverse_determinant(float **matrix, int dim);
void inverse_transpose(float **matrix, float **fac, int dim);

void cofactor(float **matrix, int dim);
void inverse_matrix(linked_list_t *list);

void subtract(linked_list_t *list);
void min_elem(linked_list_t *list);
void trace(linked_list_t *list);
void hadamard(linked_list_t *list);
void upper(linked_list_t *list);
void eig(linked_list_t *list);


#endif /* FUNCTII_2_H_ */
