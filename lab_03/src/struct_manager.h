#ifndef __MATRIX_MEMORY_MANAGER_H__
#define __MATRIX_MEMORY_MANAGER_H__

#include "matrix_struct.h"
#include "list_struct.h"

int new_matrix(matrix_t *src);
void free_memory(matrix_t *matrix);
void free_std_matrix(matrix_t *matrix, matrix_t *vector, matrix_t *result);
void free_all_memory(list_t *list_matrix, list_t *list_vector, list_t *list_result,
    matrix_t *std_matrix, matrix_t *std_vector, matrix_t *std_result);

int init_list_ptrs(int size, list_t *list);
void free_list(list_t *list);
void print_list(list_t *list, void (*print_node) (node_t));
node_t *get_by_pos(list_t *list, int pos);
void change_by_pos(node_t *node, list_t *list, int pos);

#endif
