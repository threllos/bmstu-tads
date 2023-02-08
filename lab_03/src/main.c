#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "multiplication.h"
#include "matrix_io.h"
#include "processing.h"
#include "matrix_struct.h"
#include "struct_manager.h"
#include "list_struct.h"

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__ (
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r" (high), "=r" (low)
        :: "%rax", "%rbx", "%rcx", "%rdx"
        );

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}

int main(void)
{
    setbuf(stdout, NULL);

    sparse_t sparse_matrix, sparse_vector, sparse_result;
    matrix_t std_matrix, std_vector, std_result;
    list_t matrix_list, vector_list, result_list;

    int enter_type;

    print_info();

    if (read_type(&enter_type))
        return EXIT_FAILURE;

    if (read_sizes(&sparse_matrix, &sparse_vector))
        return EXIT_FAILURE;

    int curr_size = sparse_matrix.curr_size;

    init_sizes(&sparse_vector, sparse_matrix.rows);
    init_sizes(&sparse_result, sparse_matrix.columns);

    if (init_std_matrix(&std_matrix, sparse_matrix.columns, sparse_matrix.rows))
    {
        free_memory(&std_matrix);
        return EXIT_FAILURE;
    }

    if (init_std_matrix(&std_vector, sparse_vector.columns, sparse_vector.rows))
    {
        free_memory(&std_matrix);
        free_memory(&std_vector);
        return EXIT_FAILURE;
    }

    if (init_std_matrix(&std_result, sparse_matrix.columns, sparse_vector.rows))
    {
        free_std_matrix(&std_matrix, &std_vector, &std_result);
        return EXIT_FAILURE;
    }

    int *A_matrix = malloc(sizeof(int) * sparse_matrix.curr_size);
    int *JA_matrix = malloc(sizeof(int) * sparse_matrix.curr_size);
    int *JA_vector = malloc(sizeof(int) * sparse_vector.curr_size);
    int *JA_result = malloc(sizeof(int) * sparse_matrix.columns);
    int *A_vector = malloc(sizeof(int) * sparse_vector.curr_size);
    int *A_result = malloc(sizeof(int) * sparse_result.columns);

    if (init_sparse_matrix(&sparse_matrix, A_matrix, JA_matrix, &matrix_list))
    {
        free_std_matrix(&std_matrix, &std_vector, &std_result);
        return EXIT_FAILURE;
    }

    if (init_sparse_matrix(&sparse_vector, A_vector, JA_vector, &vector_list))
    {
        free_std_matrix(&std_matrix, &std_vector, &std_result);
        return EXIT_FAILURE;
    }

    if (init_sparse_matrix(&sparse_result, A_result, JA_result, &result_list))
    {
        free_std_matrix(&std_matrix, &std_vector, &std_result);
        return EXIT_FAILURE;
    }

    if (filling_matrix(&std_matrix, &sparse_matrix, enter_type, 1))
    {
        free_all_memory(&matrix_list, &vector_list, &result_list, &std_matrix, &std_vector, &std_result);
        return EXIT_FAILURE;
    }

    if (filling_matrix(&std_vector, &sparse_vector, enter_type, 0))
    {
        free_all_memory(&matrix_list, &vector_list, &result_list, &std_matrix, &std_vector, &std_result);
        return EXIT_FAILURE;
    }

    sparse_matrix.columns = std_matrix.columns;
    sparse_matrix.rows = std_matrix.rows;

    int64_t std_start = tick();
    matrix_multiplication(&std_matrix, &std_vector, &std_result);
    int64_t std_end = tick();
    sparse_multiplication(&sparse_matrix, &std_vector, &sparse_result);
    int64_t sparse_end = tick();

    transpose(&std_matrix);

    print_enter_data(std_matrix, std_vector);
    print_result(std_result);
    print_sparse(sparse_result);
    print_data(std_matrix, sparse_matrix, std_start, std_end, sparse_end, curr_size);

    return EXIT_SUCCESS;
}
