#ifndef _ARRAY_OPERATIONS_H_
#define _ARRAY_OPERATIONS_H_

#include "structs.h"

int create_arr(array_t *array, int *max_len);
int pop_arr(array_t *stack_arr);
void push_arr(array_t *array, int num);
int print_arr(array_t stack_arr);
int result_arr(int *res);
int calculate_arr(exp_t exp);
int add_elem_arr(array_t *array, int *max_len);
void free_arr(array_t *static_arr);

#endif