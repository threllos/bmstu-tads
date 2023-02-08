#ifndef __MATRIX_STRUCT_H__
#define __MATRIX_STRUCT_H__

#include "list_struct.h"

typedef struct
{
	size_t rows;
    size_t columns;
    int curr_size;

    int *A;
    int *JA;
    list_t IA;
} sparse_t;

typedef struct
{
    int **matrix;
    size_t columns;
    size_t rows;
} matrix_t;

#endif
