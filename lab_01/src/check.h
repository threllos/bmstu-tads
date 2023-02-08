#ifndef __CHECK_H__
#define __CHECK_H__

#include <stdlib.h>
#include "bignum.h"

short_t input_check(char *float_number);
short_t check_sign(char item);
short_t result_of_mantissa_parsing_check(const char *mantissa);
short_t null_check(bignum_t result_number);
short_t integer_e_check(bignum_t integer);
short_t check_expon(char *num, size_t cur_len, size_t len);
short_t integer_check(char *integer_raw);

#endif