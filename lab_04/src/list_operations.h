#ifndef _LIST_OPERATIONS_H_
#define _LIST_OPERATIONS_H_

#include "structs.h"

int create_list(list_t **stack_list, int *max_len);
int pop_list(list_t **list);
int push_node(list_t **list, int num);
list_t* create_node(int num);
int add_elem_list(list_t **list, int *max_len);
void add_adress(arr_clear_t *arr, list_t *list);
void del_adress(arr_clear_t *arr, list_t *list);
int print_list(list_t *list);
void print_arr_adress(arr_clear_t arr);
int calculate_list(exp_t exp);
int result_list(int *res);
void free_list(list_t **list);

#endif
