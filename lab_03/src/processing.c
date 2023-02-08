#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "processing.h"
#include "matrix_struct.h"
#include "matrix_io.h"
#include "struct_manager.h"
#include "multiplication.h"
#include "list_struct.h"

void init_sizes(sparse_t *vector, size_t columns)
{
    vector->columns = columns;
    vector->rows = 1;
}

int init_sparse_matrix(sparse_t *sparse_matrix, int *A, int *JA, list_t *list)
{
    if (sparse_matrix->rows != 1)
    {
        if (init_list_ptrs(sparse_matrix->columns, list))
        {
            free_list(list);
            return EXIT_FAILURE;
        }
    }
    else
        list->ptr = NULL;

    sparse_matrix->A = A;
    sparse_matrix->JA = JA;
    sparse_matrix->IA = *list;

    return EXIT_SUCCESS;
}

int init_std_matrix(matrix_t *matrix, size_t columns, size_t rows)
{
    matrix->columns = columns;
    matrix->rows = rows;

    if (new_matrix(matrix))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void filling(matrix_t *std_matrix, int maxsize, int step)
{
    size_t irand, jrand;
    srand(time(NULL) + step * 20020201);

    for (size_t i = 0; i < maxsize; i++)
    {
        irand = 0;
        jrand = 0;

        do
        {
            irand = rand() % std_matrix->rows;
            jrand = rand() % std_matrix->columns;
        } while (std_matrix->matrix[irand][jrand] != 0);

        std_matrix->matrix[irand][jrand] = rand() % 100 + 1;
    }
}

void copy_matrix(matrix_t std_matrix, sparse_t *sparse_matrix)
{
    int k = 0;

    for (size_t i = 0; i < std_matrix.rows; i++)
    {
        if (sparse_matrix->IA.ptr != NULL)
        {
            node_t *temp_IA = get_by_pos(&sparse_matrix->IA, i);
            temp_IA->start_column_ind = k;
            change_by_pos(temp_IA, &sparse_matrix->IA, i);
        }

        for (size_t j = 0; j < std_matrix.columns; j++)
            if (std_matrix.matrix[i][j] != 0)
            {
                sparse_matrix->A[k] = std_matrix.matrix[i][j];
                sparse_matrix->JA[k] = j;
                k += 1;
            }
    }

    sparse_matrix->curr_size = k;
}

int filling_matrix(matrix_t *std_matrix, sparse_t *sparse_matrix, int enter_type, int mv_type)
{
    if (enter_type)
    {
        if (mv_type == 1)
        {
            if (read_matrix(std_matrix, sparse_matrix->curr_size))
                return EXIT_FAILURE;
        }
        else if (mv_type == 0)
        {
            if (read_vector(std_matrix, sparse_matrix->curr_size))
                return EXIT_FAILURE;
        }
        else
            return EXIT_FAILURE;
    }
    else
        filling(std_matrix, sparse_matrix->curr_size, mv_type);

    if (mv_type)
        transpose(std_matrix);

    copy_matrix(*std_matrix, sparse_matrix);

    return EXIT_SUCCESS;
}
