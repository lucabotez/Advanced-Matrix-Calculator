// Copyright Croitoru Constantin-Bogdan Grupa 314CA 2022-2023
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

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


typedef struct ll_node_t ll_node_t ;
typedef struct linked_list_t linked_list_t;

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
	linked_list_t *list = malloc(sizeof(linked_list_t));
	DIE(list == NULL, "Nu s-a putut aloca");
	list->data_size = data_size;
	list->size = 0;
	list->head = NULL;
	return list;
}

void ll_add_nth_node(linked_list_t *list, int n, int **new_data, int nr_l, int nr_c) {
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

unsigned int
ll_get_size(linked_list_t *list) {
	return (unsigned int)list->size;
}

void ll_free(linked_list_t **pp_list) {
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
