#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functii_2.h"
// auxiliary function  that calculates the determinant for float matrices
float float_determinant(float **matrix, int dim)
{
	float ratio, det = 1;
	float **aux;
    int i, j, k;

	aux = (float **)malloc(dim * sizeof(float *));
	for(i = 0; i < dim; i++)
		aux[i] = (float *)malloc(dim * sizeof(float));

	for(i = 0; i < dim; i++)
		for(j = 0; j < dim; j++)
			aux[i][j] = matrix[i][j];

    for(i = 0; i < dim; i++) {
        if(aux[i][i] == 0.0) {
            printf("Mathematical Error!");
            exit(0);
        }

        for(j = i + 1; j < dim; j++) {
            ratio = aux[j][i] / aux[i][i];

            for(k = 0;k < dim; k++)
                aux[j][k] = aux[j][k] - ratio * aux[i][k];
        }
    }

	for(i = 0;i < dim; i++)
        det = det * matrix[i][i];

	for(i = 0; i < dim; i++)
		free(aux[i]);
	free(aux);

    return det;
}

// auxiliary function used in the determinant calculation
float inverse_determinant(float **matrix, int dim)
{
	float s = 1, det = 0, **b;
	int i, j, m, n, c;
	b = (float **)malloc(dim * sizeof(float *));
	for(i = 0; i < dim; i++)
		b[i] = (float *)malloc(dim * sizeof(float));

	if (dim == 1) {
		return (matrix[0][0]);
	} else {
		det = 0;
		for (c = 0; c < dim; c++) {
		m = 0;
		n = 0;
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++){
				b[i][j] = 0;
				if (i != 0 && j != c){
					b[m][n] = matrix[i][j];
					if (n < (dim - 2)) {
						n++;
					} else {
						n = 0;
						m++;
					}
				}
			}
		det = det + s * (matrix[0][c] * inverse_determinant(b, dim - 1));
		s = -1 * s;
		}
	}
	for(i = 0; i < dim; i++)
		free(b[i]);
	free(b);

	return (det);	
}

// auxiliary function that pieces all the calculated elements together in order
// to determine the inverse
void inverse_transpose(float **matrix, float **fac, int dim)
{
	int i, j;
	float **b, **inverse, d;
	b = (float **)malloc(dim * sizeof(float *));
	inverse = (float **)malloc(dim * sizeof(float *));
	for (int i = 0; i < dim; i++) {
		b[i] = (float *)malloc(dim * sizeof(float));
		inverse[i] = (float *)malloc(dim * sizeof(float));
	}
 
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			b[i][j] = fac[j][i];

	d = inverse_determinant(matrix, dim);
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			inverse[i][j] = b[i][j] / d;

	printf("The inverse is : \n");

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++)
			printf("%f ", inverse[i][j]);

		printf("\n");
	}

	for(i = 0; i < dim; i++) {
		free(matrix[i]);
		free(inverse[i]);
		free(b[i]);
		free(fac[i]);
	}
	free(matrix);
	free(inverse);
	free(b);
	free(fac);
}

// auxiliary function that calculates the adjoint matrix
void cofactor(float **matrix, int dim)
{
	float **b, **fac;
	b = (float **)malloc(dim * sizeof(float *));
	fac = (float **)malloc(dim * sizeof(float *));
	for (int i = 0; i < dim; i++) {
		b[i] = (float *)malloc(dim * sizeof(float));
		fac[i] = (float *)malloc(dim * sizeof(float));
	}

	int p, q, m, n, i, j;
	for (q = 0; q < dim; q++) {
		for (p = 0; p < dim; p++) {
    		m = 0;
    		n = 0;
     		for (i = 0; i < dim; i++) {
    			for (j = 0; j < dim; j++) {
    				if (i != q && j != p) {
            			b[m][n] = matrix[i][j];
            			if (n < (dim - 2)) {
            				n++;
						} else {
            				n = 0;
            				m++;
            			}
            		}
        		}
      		}
    		fac[q][p] = pow(-1, q + p) * inverse_determinant(b, dim - 1);
		}
	}
	for(i = 0; i < dim; i++)
		free(b[i]);
	free(b);

	inverse_transpose(matrix, fac, dim);
}

void inverse_matrix(linked_list_t *list) {
	int index;
	fscanf(stdin, "%d", &index);

	// getting the required matrix from the list and its dimensions
	int **matrix = ll_get_nth_node(list, index)->data;
	int rows = ll_get_nth_node(list, index)->linie;
	int cols = ll_get_nth_node(list, index)->coloana;

	// transforming the matrix to float
	float **f_matrix = (float **)malloc(rows * sizeof(float *));
	for (int i = 0; i < rows; i++)
		f_matrix[i] = (float *)malloc(cols * sizeof(float));

	for (int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			f_matrix[i][j] = matrix[i][j];

	// checking if the inverse operation is mathematically possible
	if(rows != cols || float_determinant(f_matrix, rows) == 0) {
		printf("The matrix cannot be inversed");
		return;
	}

	// calculating the inverse
	cofactor(f_matrix, rows);

}

void subtract(linked_list_t *list)
{
	int index1, index2;
	fscanf(stdin, "%d%d", &index1, &index2);

	// checking the indexes
	if(index1 >= list->size || index2 >= list->size) {
		printf("Invalid index.\n");
		return;
	}

	int **matrix1 = ll_get_nth_node(list, index1)->data;
	int rows1 = ll_get_nth_node(list, index1)->linie;
	int cols1 = ll_get_nth_node(list, index1)->coloana;

	int **matrix2 = ll_get_nth_node(list, index2)->data;
	int rows2 = ll_get_nth_node(list, index2)->linie;
	int cols2 = ll_get_nth_node(list, index2)->coloana;

	// check if the subtraction is possible
	if(rows1 != rows2 || cols1 != cols2) {
		printf("Cannot perform subtraction.\n");
		return;
	}

	// temporary matrix in which we store the result
	int **result = (int **)malloc(rows1 * sizeof(int *));
	for(int i = 0; i < cols1; i++)
		result[i] = (int *)malloc(cols1 * sizeof(int));

	// calculating the result
	for(int i = 0; i < rows1; i++)
		for(int j = 0; j < cols1; j++)
			result[i][j] = matrix1[i][j] - matrix2[i][j];
	
	// printing the matrix
	for(int i = 0; i < rows1; i++) {
		for(int j = 0; j < cols1; j++)
			printf("%d ", result[i][j]);
		printf("\n");
	}

	// freeing the temporary matrix
	for(int i = 0; i < rows1; i++)
		free(result[i]);
	free(result);
}

void min_elem(linked_list_t *list)
{
	int index;
	fscanf(stdin, "%d", &index);

	// checking the index
	if(index >= list->size) {
		printf("Invalid index.\n");
		return;
	}

	int **matrix = ll_get_nth_node(list, index)->data;
	int rows = ll_get_nth_node(list, index)->linie;
	int cols = ll_get_nth_node(list, index)->coloana;

	// calculating the minimum value
	int min = matrix[0][0];
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			if(matrix[i][j] < min)
				min = matrix[i][j];

	printf("%d\n", min);
}

void trace(linked_list_t *list)
{
	int index;
	fscanf(stdin, "%d", &index);

	// checking the index
	if(index >= list->size) {
		printf("Invalid index.\n");
		return;
	}

	int **matrix = ll_get_nth_node(list, index)->data;
	int rows = ll_get_nth_node(list, index)->linie;
	int cols = ll_get_nth_node(list, index)->coloana;

	// checking if the matrix is square
	if(rows != cols) {
		printf("Cannot calculate the trace on a non-square matrix.\n");
		return;
	}

	// calculating the trace
	int tr = 0;
	for(int i = 0; i < rows; i++)
		tr += matrix[i][i];

	printf("%d\n", tr);
}

void hadamard(linked_list_t *list)
{
	int index1, index2;
	fscanf(stdin, "%d%d", &index1, &index2);

	// checking the indexes
	if(index1 >= list->size || index2 >= list->size) {
		printf("Invalid index.\n");
		return;
	}

	int **matrix1 = ll_get_nth_node(list, index1)->data;
	int rows1 = ll_get_nth_node(list, index1)->linie;
	int cols1 = ll_get_nth_node(list, index1)->coloana;

	int **matrix2 = ll_get_nth_node(list, index2)->data;
	int rows2 = ll_get_nth_node(list, index2)->linie;
	int cols2 = ll_get_nth_node(list, index2)->coloana;

	// checking if the dimensions are equal
	if(rows1 != rows2 || cols1 != cols2) {
		printf("Cannot perform hadamard product.\n");
		return;
	}

	// temporary matrix in which we store the result
	int **result = (int **)malloc(rows1 * sizeof(int *));
	for(int i = 0; i < cols1; i++)
		result[i] = (int *)malloc(cols1 * sizeof(int));

	// calculating the result
	for(int i = 0; i < rows1; i++)
		for(int j = 0; j < cols1; j++)
			result[i][j] = matrix1[i][j] * matrix2[i][j];
	
	// printing the matrix
	for(int i = 0; i < rows1; i++) {
		for(int j = 0; j < cols1; j++)
			printf("%d ", result[i][j]);
		printf("\n");
	}

	// freeing the temporary matrix
	for(int i = 0; i < rows1; i++)
		free(result[i]);
	free(result);	
}

void upper(linked_list_t *list)
{
	int index;
	fscanf(stdin, "%d", &index);

	// checking the index
	if(index >= list->size) {
		printf("Invalid index.\n");
		return;
	}

	int **matrix = ll_get_nth_node(list, index)->data;
	int rows = ll_get_nth_node(list, index)->linie;
	int cols = ll_get_nth_node(list, index)->coloana;

	// checking if the matrix is square
	if(rows != cols) {
		printf("Cannot calculate the upper triangular matrix on a non-square one.\n");
		return;
	}

	// transforming the matrix to a float one
	float **f_matrix;
	f_matrix = (float **)malloc(rows * sizeof(float *));
	for(int i = 0; i < cols; i++)
		f_matrix[i] = (float *)malloc(cols * sizeof(float));

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			f_matrix[i][j] = matrix[i][j];

	// applying the gaussian elimination algorithm
	float ratio;
	for(int i = 0; i < rows; i++) {
		if(f_matrix[i][i] == 0.0) {
			printf("Cannot perform gaussian elimination.\n");
			return;
		}
		for(int j = i + 1; j < cols; j++) {
			ratio = f_matrix[j][i] / f_matrix[i][i];

			for(int k = 0; k < cols; k++)
				f_matrix[j][k] = f_matrix[j][k] - ratio * f_matrix[i][k];
		}
	}

	// printing the upper triangular matrix
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++)
			printf("%.3f ", f_matrix[i][j]);
		printf("\n");
	}

	// freeing the temporary matrix
	for(int i = 0; i < rows; i++)
		free(f_matrix[i]);
	free(f_matrix);
}

void eig(linked_list_t *list)
{
	int index;
	fscanf(stdin, "%d", &index);

	// checking the index
	if(index >= list->size) {
		printf("Invalid index.\n");
		return;
	}

	int **matrix = ll_get_nth_node(list, index)->data;
	int rows = ll_get_nth_node(list, index)->linie;
	int cols = ll_get_nth_node(list, index)->coloana;

	// checking if the matrix is square
	if(rows != cols) {
		printf("Cannot calculate the trace on a non-square matrix.\n");
		return;
	}

	// applying direct power method algorithm to determine the dominant eigenvalue
	float *x, *x_new;
	x = (float *)malloc(rows * sizeof(float));
	for(int i = 0; i < rows; i++)
		x[i] = 1;

	x_new = (float *)malloc(rows * sizeof(float));

	float temp, lambda_new, lambda_old, err;
	err = 0.001;
	lambda_old = 1;

	int i, j;

	while(1) {
		for(i = 0; i < rows; i++) {
			temp = 0.0;
			for(j = 0; j < rows; j++)
				temp += (float)matrix[i][j] * x[j];

			x_new[i] = temp;
		}

		for(i = 0; i < rows; i++)
			x[i] = x_new[i];

		lambda_new = fabs(x[0]);
		for(i = 1; i < rows; i++)
			if(fabs(x[i]) > lambda_new)
				lambda_new = fabs(x[i]);

		for(i = 0; i < rows; i++)
			x[i] = x[i] / lambda_new;

		if(fabs(lambda_new - lambda_old) <= err)
			break;

		lambda_old = lambda_new;
	}

	printf("%f\n", lambda_new);
	free(x);
	free(x_new);
}
