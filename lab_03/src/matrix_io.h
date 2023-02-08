#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdint.h>

#include "matrix_struct.h"

void print_info();
void print_enter_data(matrix_t matrix, matrix_t vector);
void print_result(matrix_t std_matrix);
void print_sparse(sparse_t matrix);
void print_data(matrix_t std_matrix, sparse_t sparse_matrix,
    int64_t std_start, int64_t std_end, int64_t sparse_end, int curr_size);

int read_sizes(sparse_t *matrix, sparse_t *vector);
int read_type(int *enter_type);
int read_matrix(matrix_t *std_matrix, int maxsize);
int read_vector(matrix_t *std_matrix, int maxsize);

#endif
