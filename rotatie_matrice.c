// Copyright Croitoru Constantin-Bogdan Grupa 314CA 2022-2023
#include "rotatie_matrice.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rotatie_90_grade(linked_list_t* list, int indice) {
	ll_node_t* nod = ll_get_nth_node(list, indice);
	int nr_l = nod->linie;
	int nr_c = nod->coloana;
	int** aux;
	aux = malloc(nr_l * sizeof(int*));
	for (int i = 0; i < nr_l; i++)
		aux[i] = malloc(nr_l * sizeof(int));

	// se retine in aux matricea rotita
	for (int i = 0; i < nr_l; i++)
		for (int j = 0; j < nr_c; j++)
			aux[j][i] = nod->data[nr_l - 1 - i][j];

	for (int i = 0; i < nr_l; i++)
		for (int j = 0; j < nr_c; j++)
			nod->data[i][j] = aux[i][j];

	for (int i = 0; i < nr_l; i++)
		free(aux[i]);
	free(aux);
}

// functia realizeaza operatia de rotatie
void rotatie(linked_list_t* list) {
	int grade, indice;
	scanf("%d", &indice);
	scanf("%d", &grade);
	ll_node_t* node = ll_get_nth_node(list, indice);

	if (node->linie != node->coloana) {
		printf("Maticea trebuie sa fie patratica");
		return;
	}

	if (grade % 90 != 0 || grade > 360 || grade < -360) {
		printf("Unghi de rotație nesuportat\n");
		return;
	}

	if (grade / 90 == 1 || grade / 90 == -3)
		rotatie_90_grade(list, indice);

	if (grade / 90 == 2 || grade / 90 == -2) {
		rotatie_90_grade(list, indice);
		rotatie_90_grade(list, indice);
	}

	if (grade / 90 == 3 || grade / 90 == -1) {
		rotatie_90_grade(list, indice);
		rotatie_90_grade(list, indice);
		rotatie_90_grade(list, indice);
	}
}
