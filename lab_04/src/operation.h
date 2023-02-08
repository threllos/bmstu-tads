#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "structs.h"

int read_expression(exp_t *expression);
int check_operation(char sym);
int check_priority(int sym);
int read_num_from_arr_char(char *sym, size_t *ind);
int translate_operation(char sym);

#endif