// Croiotru Bogdan
#ifndef LIST_H_
#define LIST_H_

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
node_create(int **new_data, unsigned int data_size, int nr_l, int nr_c);

linked_list_t *ll_create(unsigned int data_size);

void ll_add_nth_node(linked_list_t *list, int n, int **new_data, int nr_l, int nr_c);
ll_node_t *ll_remove_nth_node(linked_list_t *list, int n);
unsigned intll_get_size(linked_list_t *list);

void ll_free(linked_list_t **pp_list);
ll_node_t *ll_get_nth_node(linked_list_t *list, unsigned int n);
void eliberare_memorie_matrice(int **mat, int nr_l);

#endif /* LIST_H_ */
