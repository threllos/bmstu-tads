#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

#include <stdlib.h>
#include "bignum.h"

short_t to_exp(char *raw_int);
void normalization(bignum_t *number);
void multiplication(bignum_t *int_number, bignum_t *float_number,
					bignum_t *result_number);
void null_round(bignum_t *result_number, size_t index);
void rounding(bignum_t *number, size_t end_ind, short_t flag);
void rounding2(bignum_t *number, size_t end_ind, short_t flag);

#endif

