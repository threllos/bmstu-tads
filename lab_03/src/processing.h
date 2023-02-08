#ifndef __PROCESSING_H__
#define __PROCESSING_H__

#include "matrix_struct.h"

void init_sizes(sparse_t *vector, size_t columns);
int init_sparse_matrix(sparse_t *sparse_matrix, int *A, int *IA, list_t *list);
int init_std_matrix(matrix_t *matrix, size_t columns, size_t rows);
int filling_matrix(matrix_t *std_matrix, sparse_t *sparse_matrix, int enter_type, int mv_type);
void copy_matrix(matrix_t std_matrix, sparse_t *sparse_matrix);

#endif
