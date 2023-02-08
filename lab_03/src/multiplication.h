#ifndef __MULTIPLICATION_H__
#define __MULTIPLICATION_H__

#include "matrix_struct.h"

void matrix_multiplication(matrix_t *matrix, matrix_t *vector, matrix_t *result);
void transpose(matrix_t *matrix);
void sparse_multiplication(sparse_t *matrix, matrix_t *vector, sparse_t *result);

#endif
