#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdlib.h>
#include "bignum.h"

short_t parse(const char *raw_number, bignum_t *parsed_number);
int is_not_digit(char sym);
int char_to_int(char sym);
short_t post_process(bignum_t integer_parsed, bignum_t float_parsed,
                   bignum_t *result_number, short_t fl);
void null_counter(const char *mantissa, size_t index, size_t *counter,
				  short_t mode);

#endif

