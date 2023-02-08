#include <stdlib.h>

#include "struct_manager.h"
#include "matrix_struct.h"

int new_matrix(matrix_t *src)
{
    src->matrix = malloc(src->rows * sizeof(int *));

    if (src->matrix == NULL)
        return EXIT_FAILURE;

    for (size_t i = 0; i < src->rows; i++)
    {
        *(src->matrix + i) = malloc(src->columns * sizeof(int));

        if (*(src->matrix + i) == NULL)
            return EXIT_FAILURE;
    }

    for (size_t i = 0; i < src->rows; i++)
        for (size_t j = 0; j < src->columns; j++)
            src->matrix[i][j] = 0;

    return EXIT_SUCCESS;
}

void free_memory(matrix_t *src)
{
    for (size_t i = 0; i < src->rows; i++)
    {
        free(src->matrix[i]);
        src->matrix[i] = NULL;
    }

    free(src->matrix);
    src->matrix = NULL;
}

void free_std_matrix(matrix_t *matrix, matrix_t *vector, matrix_t *result)
{
    free_memory(matrix);
    free_memory(vector);
    free_memory(result);
}

void free_all_memory(list_t *list_matrix, list_t *list_vector, list_t *list_result,
     matrix_t *std_matrix, matrix_t *std_vector, matrix_t *std_result)
{
    free_list(list_matrix);
    free_list(list_vector);
    free_list(list_result);
    free_std_matrix(std_matrix, std_vector, std_result);
}

int init_list_ptrs(int size, list_t *list)
{
    list->ptr = NULL;

    for (size_t i = 0; i < size; i++)
    {
        node_t *curr_element = malloc(sizeof(node_t));

        if (curr_element == NULL)
            return EXIT_FAILURE;

        curr_element->next_element = list->ptr;
        curr_element->start_column_ind = -1;
        list->ptr = curr_element;
    }

    return EXIT_SUCCESS;
}

void free_list(list_t *list)
{
    node_t *curr, *temp;
    curr = list->ptr;

    while (curr->next_element != NULL)
    {
        temp = curr->next_element;
        free(curr);
        curr = temp;
    }

    free(temp);
}

void print_list(list_t *list, void (*print_node) (node_t))
{
    node_t *node = list->ptr;

    if (node == NULL)
        return;

    while (node->next_element != NULL)
    {
        (*print_node)(*node);
        node = node->next_element;
    }

    (*print_node)(*node);
}

node_t *get_by_pos(list_t *list, int pos)
{
    node_t *temp_node = list->ptr;

    for (int i = 0; i < pos; i++)
    {
        if (temp_node->next_element == NULL)
            return temp_node;

        temp_node = temp_node->next_element;
    }

    return temp_node;
}

void change_by_pos(node_t *node, list_t *list, int pos)
{
    node_t *temp_node = get_by_pos(list, pos);
    *temp_node = *node;
}

