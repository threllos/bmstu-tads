#include <string.h>
#include <stdio.h>

#include "multiplication.h"
#include "processing.h"
#include "struct_manager.h"
#include "matrix_struct.h"

void matrix_multiplication(matrix_t *matrix, matrix_t *vector, matrix_t *result)
{
    for (size_t i = 0; i < matrix->rows; i++)
        for (size_t j = 0; j < matrix->columns; j++)
            result->matrix[0][i] += matrix->matrix[i][j] * vector->matrix[0][j];
}

void sparse_multiplication(sparse_t *matrix, matrix_t *vector, sparse_t *result)
{
    int k, curr_sum;
    node_t *temp_IA = get_by_pos(&matrix->IA, 0);
    node_t *temp_IA_next = NULL;
    result->curr_size = 0;

    for (size_t i = 0; i < matrix->rows - 1; i++)
    {
        temp_IA_next = temp_IA->next_element;
        curr_sum = 0;

        for (size_t j = temp_IA->start_column_ind; j < temp_IA_next->start_column_ind; j++)
            curr_sum += matrix->A[j] * vector->matrix[0][matrix->JA[j]];

        if (curr_sum != 0)
        {
            result->A[result->curr_size] = curr_sum;
            result->JA[result->curr_size] = i;
            result->curr_size++;
        }

        temp_IA = temp_IA_next;
        k = temp_IA->start_column_ind;
    }

    curr_sum = 0;
    for (size_t i = k; i < matrix->curr_size; i++)
        curr_sum += matrix->A[i] * vector->matrix[0][matrix->JA[i]];

    if (curr_sum != 0)
    {
        result->A[result->curr_size] = curr_sum;
        result->JA[result->curr_size] = matrix->rows - 1;
        result->curr_size++;
    }
}

void transpose_matrix(matrix_t *src, matrix_t *dst)
{
    for (size_t i = 0; i < src->rows; i++)
        for (size_t j = 0; j < src->columns; j++)
            dst->matrix[j][i] = src->matrix[i][j];
}

void transpose(matrix_t *matrix)
{
    matrix_t tmp;
    tmp.columns = matrix->rows;
    tmp.rows = matrix->columns;

    new_matrix(&tmp);
    transpose_matrix(matrix, &tmp);

    free_memory(matrix);
    *matrix = tmp;
}
