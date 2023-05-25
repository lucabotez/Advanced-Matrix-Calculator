// Copyright Croitoru Constantin-Bogdan Grupa 314CA 2022-2023
#include "functii.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define D_MOD 10007

int **alocare_mat(int nr_l, int nr_c) {
    int **aux;
    aux = malloc(nr_l * sizeof(int *));
    DIE(!aux, "Alocare esuata");

    for (int i = 0; i < nr_l; i++) {
        aux[i] = malloc(nr_c * sizeof(int));
        DIE(!aux[i], "Alocare esuata");
    }
    return aux;
}

double **alocare_mat_double(int nr_l, int nr_c) {
    double **aux;
    aux = malloc(nr_l * sizeof(double *));
    DIE(!aux, "Alocare esuata");

    for (int i = 0; i < nr_l; i++) {
        aux[i] = malloc(nr_c * sizeof(double));
        DIE(!aux[i], "Alocare esuata");
    }
    return aux;
}

// functia calculeaza suma a doua matrici si returneaza rezultatul
int **adunare_matrici(int **a, int **b, int nl, int nc) {
    int **aux = alocare_mat(nl, nc);

    for (int i = 0; i < nl; i++)
        for (int j = 0; j < nc; j++)
            aux[i][j] = (a[i][j] + b[i][j]) % D_MOD;

    return aux;
}

void aplicare_suma(linked_list_t *list) {
    int **aux;
    int indice1, indice2;
    scanf("%d%d", &indice1, &indice2);
    ll_node_t *node1, *node2;
    node1 = ll_get_nth_node(list, indice1);
    node2 = ll_get_nth_node(list, indice2);
    if (node1->coloana != node2->coloana) {
        printf("Matricile trebuie sa aiba aceeasi dimensiune\n");
        return;
    }

    if (node1->linie != node2->linie) {
        printf("Matricile trebuie sa aiba aceeasi dimensiune\n");
        return;
    }

    aux = adunare_matrici(node1->data, node2->data, node1->linie, node1->coloana);

    for (int i = 0; i < node1->linie; i++) {
        for (int j = 0; j < node1->coloana; j++)
            printf("%d ", aux[i][j]);
        printf("\n");
    }

    for (int i = 0; i < node1->linie; i++)
        free(aux[i]);
    free(aux);
}

// functia determina determinantul unei matrici
int aflare_determinant(int **mat, int n) {
    int **aux2;
    // se retine maxima sa ne se piarda valoarea
    aux2 = alocare_mat(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            aux2[i][j] = mat[i][j];

    int num1, num2, det = 1, index, total = 1;
    int temp[n + 1];


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

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat[i][j] = aux2[i][j];

    for (int i = 0; i < n; i++)
        free(aux2[i]);
    free(aux2);

    return (det / total);
}

int determinare_rang(int **mat, int nr_l, int nr_c) {
    int rang = 0;
    int mini = nr_l;
    if (mini > nr_c)
        mini = nr_c;
    int **aux;
    aux = malloc(mini * sizeof(int *));
    for (int i = 0; i < mini; i++)
        aux[i] = calloc(mini, sizeof(int));

    if (mat[0][0])
        rang = 1;

    for (int i = 1; i < mini; i++) {
        for (int j = 0; j <= i; j++)
            for (int k = 0; k <= i; k++)
                aux[j][k] = mat[j][k];
        int det = aflare_determinant(aux, i + 1);
        if (det)
            rang = i + 1;
    }

    for (int i = 0; i < mini; i++)
        free(aux[i]);
    free(aux);
    return rang;
}

// functia returneaza elementul maxim dintr-o matrice
int determinare_maxim(int **mat, int nr_l, int nr_c) {
    int maxi = mat[0][0];
    for (int i = 0; i < nr_l; i++)
        for (int j = 0; j < nr_c; j++)
            if (maxi < mat[i][j])
                maxi = mat[i][j];

    return maxi;
}

// functia ridica fiecare element din matrice la o anumita putere
// rezultatul este retinut in aceeasi matrice
void ridicare_fiecare_elem(ll_node_t *nod, int nr_l, int nr_c, int k) {
    int **aux;
    int **mat = nod->data;
    aux = alocare_mat(nr_l, nr_c);

    if (k == 0) {
        for (int i = 0; i < nr_l; i++)
            for (int j = 0; j < nr_c; j++)
                aux[i][j] = 1;
    } else {
        for (int i = 0; i < nr_l; i++)
            for (int j = 0; j < nr_c; j++) {
                aux[i][j] = mat[i][j];
                for (int p = 0; p < k - 1; p++)
                    aux[i][j] *= mat[i][j];
            }
    }
    for (int i = 0; i < nr_l; i++)
        for (int j = 0; j < nr_c; j++)
            nod->data[i][j] = aux[i][j];

    for (int i = 0; i < nr_l; i++)
        free(aux[i]);
    free(aux);
}

void doolittle_algoritm(double **A, int n) {
    double **L;
    double **U;
    L = alocare_mat_double(n, n);
    U = alocare_mat_double(n, n);
    // Verificăm dacă matricea A este pătratică
    if (n < 1) {
        printf("Matricea nu este patratica.");
        return;
    }

    // Inițializăm matricele L și U
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                L[i][j] = 1;
            } else {
                L[i][j] = 0;
            }
            U[i][j] = 0;
        }
    }

    // Iterăm pe pivoti
    for (int p = 0; p < n; p++) {
        // Calculăm linia p din U
        for (int j = p; j < n; j++) {
            double sigma = 0;
            for (int k = 0; k < p; k++) {
                sigma += L[p][k] * U[k][j];
            }
            U[p][j] = A[p][j] - sigma;
        }

        // Calculăm coloana p din L
        for (int i = p + 1; i < n; i++) {
            double sigma = 0;
            for (int k = 0; k < p; k++) {
                sigma += L[i][k] * U[k][p];
            }
            L[i][p] = (A[i][p] - sigma) / U[p][p];
        }
    }
    // L
    printf("L este:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6f ", L[i][j]);
        printf("\n");
    }

    // U
    printf("U este:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6f ", U[i][j]);
        printf("\n");
    }

    for (int i = 0; i < n; i++)
        free(L[i]);
    free(L);

    for (int i = 0; i < n; i++)
        free(U[i]);
    free(U);
}
