#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "parse.h"
#include "errors.h"
#include "check.h"
#include "operations.h"

void default_data(bignum_t *parsed_number)
{
    parsed_number->sign = '+';
    parsed_number->degree = 0;
    parsed_number->point_index = 0;
    memset(parsed_number->mantissa, '\0', sizeof(parsed_number->mantissa));
}

int char_to_int(const char sym)
{
    return sym - '0';
}

int is_not_digit(const char sym)
{
    if ((int) sym < 48 || (int) sym > 57)
        return 1;

    return 0;
}

void get_sign_parse(const char *raw_number, bignum_t *parsed_number, size_t *index)
{
    if (is_not_digit(raw_number[*index]))
    {
        parsed_number->sign = raw_number[*index];
        (*index)++;
    }
}

short_t parse_mantissa(const char *raw_number, bignum_t *parsed_number, size_t *index)
{
    size_t length = strlen(parsed_number->mantissa);

    if (raw_number[*index] == '\0')
        return 0;

    if (raw_number[*index] == '.')
        parsed_number->point_index = *index;
    else if (raw_number[*index] == 'e' || raw_number[*index] == 'E')
    {
        if (parsed_number->point_index)
            parsed_number->point_index = *index - parsed_number->point_index - 1;

        (*index)++;
        parsed_number->mantissa[length] = '\0';
        return 0;
    }
    else if (length >= MAX_MANTISSA - 1)
        return PARSING_MANTISSA_ERROR;
    else
        parsed_number->mantissa[length] = raw_number[*index];

    (*index)++;

    return parse_mantissa(raw_number, parsed_number, index);
}

short_t parse_degree(const char *raw_number, bignum_t *parsed_number,
                       size_t *index, char *temp_degree, size_t length)
{
    if (raw_number[*index] == '\0')
    {
        temp_degree[length] = '\0';
        parsed_number->degree = atoi(temp_degree);
        return 0;
    }
    if (length > MAX_DEGREE_LEN - 3)
    {
        if (isdigit(temp_degree[0]))
            return 0;

        return PARSING_DEGREE_OUT_RANGE;
    }
    temp_degree[length] = raw_number[*index];
    *index += 1;

    return parse_degree(raw_number, parsed_number, index, temp_degree, length + 1);
}

short_t parse(const char *raw_number, bignum_t *parsed_number)
{
    size_t temp_index = 0;
    short_t code_error = 0;

    char temp_degree[MAX_DEGREE_LEN] = {'\0'};

    default_data(parsed_number);

    get_sign_parse(raw_number, parsed_number, &temp_index);

    code_error = parse_mantissa(raw_number, parsed_number, &temp_index);
    if (code_error != 0)
        return code_error;

    code_error = parse_degree(raw_number, parsed_number, &temp_index, temp_degree, 0);
    if (code_error != 0)
        return code_error;

    return 0;
}

void find_degree(bignum_t *result, int index)
{
    if (result->mantissa[index] == '+' || result->mantissa[index] == '-')
    {
        result->mantissa[index] = '\0';
        return;
    }

    find_degree(result, index + 1);
}

void null_counter(const char *mantissa, size_t index,
                  size_t *counter, short_t mode)
{
    while (mantissa[index] == '0')
    {
        (*counter)++;
        if (index == 0 && mode == -1)
            return;

        index += mode;
    }
}

size_t final_degree_addition(const char *mantissa)
{
    size_t len = strlen(mantissa) - 1;
    size_t k = 0;

    while (mantissa[len] == '0' && len)
    {
        k += 1;
        len -= 1;
    }

    return k;
}

short_t post_process(bignum_t integer_parsed,
                   bignum_t float_parsed,
                   bignum_t *result_number,
                   short_t fl)
{
    size_t null_cnt_behind = 0, null_cnt_before = 0;
    size_t len = strlen(result_number->mantissa);

    find_degree(result_number, 0);

    null_counter(result_number->mantissa, 0, &null_cnt_before, 1);
    null_counter(result_number->mantissa, len - 1, &null_cnt_behind, -1);

    result_number->mantissa[len - null_cnt_behind] = '\0';
    strcpy(result_number->mantissa, (result_number->mantissa + null_cnt_before));

    if (fl == 1)
        null_cnt_behind = final_degree_addition(integer_parsed.mantissa);

    result_number->degree = float_parsed.degree + null_cnt_behind +
                            (strlen(result_number->mantissa) - result_number->point_index);


    if (null_check(*result_number))
        return 0;

    if (strlen(result_number->mantissa) == 60)
        result_number->mantissa[59] = '0';

    if (abs(result_number->degree) > MAX_DEGREE_VAL)
        return PARSING_RANGE_ERROR;

    null_round(result_number, strlen(result_number->mantissa) - 1);
    if (strlen(result_number->mantissa) > (MAX_MANTISSA - 1))
        return PARSING_OVERFLOW;

    return 0;
}