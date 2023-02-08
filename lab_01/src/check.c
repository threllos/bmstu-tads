#include "check.h"
#include "errors.h"
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

short_t e_counter(const char item, short_t *e_counter)
{
    if (item == 'e' || item == 'E')
        *e_counter += 1;

    if (*e_counter > 1)
        return -1;

    return 0;
}

short_t dots_counter(const char item, short_t *dots)
{
    if (item == '.')
        *dots += 1;

    if (*dots > 1)
        return -1;
    return 0;
}

short_t check_allowed(const char item)
{
    if (item != '-' && item != '+' && item != '.' && item != 'e' && item != 'E' && (is_not_digit(item)))
        return -1;

    return 0;
}

short_t sign_check(const char *num, const size_t index)
{
    if (num[index] == '+' || num[index] == '-')
    {
        if (index == 0)
            return 0;

        if (!(num[index - 1] == 'E' || num[index - 1] == 'e'))
            return -1;
    }
    return 0;
}

short_t sign_after_e_check(const char e, const char sign)
{
    if ((e == 'e' || e == 'E'))
        if (!(sign == '+' || sign == '-'))
            return -1;
    return 0;
}

short_t dots_in_degree_check(const char item, short_t e_counter)
{
    if (item == '.' && e_counter != 0)
        return -1;
    return 0;
}

short_t check_sign(const char item)
{
    if (item != '-' && item != '+')
        return -1;
    return 0;
}


short_t integer_check(char *integer_raw)
{
    short_t e_flag = 0;
    short_t sign_flag = 0;
    for (int i = 0; i < strlen(integer_raw); i++)
        if (!isdigit(integer_raw[i]))
        {
            if ((integer_raw[i] == 'E' || integer_raw[i] == 'e') && e_flag == 0)
            {
                e_flag = 1;
                continue;
            }
            else if ((integer_raw[i] == '-' || integer_raw[i] == '+') && sign_flag == 0)
            {
                sign_flag = 1;
                continue;
            }
            else
                return 1;
        }

    return 0;
}

short_t input_check(char *float_number)
{
    short_t index = 0, es = 0, dots = 0;
    char item;

    while (float_number[index] != '\0')
    {
        item = float_number[index];

        if (check_allowed(item) != 0)
            return INPUT_FORBIDDEN_SYMBOL;
        if (e_counter(item, &es) != 0)
            return INPUT_E_ERROR;
        if (sign_check(float_number, index) != 0)
            return INPUT_SIGN_ERROR;
        if (sign_after_e_check(item, float_number[index + 1]) != 0)
            return INPUT_SIGN_ERROR;
        if (dots_in_degree_check(item, es) != 0)
            return INPUT_DOTS_IN_DEGREE;
        if (dots_counter(item, &dots) != 0)
            return INPUT_DOTS_IN_MANTISSA;
        index += 1;
    }

    if (check_sign(float_number[0]) != 0)
        return INPUT_SIGN_ERROR;

    if (item == 'e' || item == 'E')
        return INPUT_E_ERROR;

    if (item == '+' || item == '-')
        return INPUT_SIGN_ERROR;

    return 0;
}

short_t result_of_mantissa_parsing_check(const char *mantissa)
{
    if (!mantissa[0])
        return -1;
    return 0;
}

short_t null_check(const bignum_t result_number)
{
    size_t len = strlen(result_number.mantissa);

    for (int i = 0; i < len; i++)
        if (result_number.mantissa[i] != '0')
            return 0;

    return 1;
}

short_t integer_e_check(bignum_t integer)
{

    size_t counter1 = 0;
    size_t counter2 = 0;
    null_counter(integer.mantissa, 0, &counter1, 1);
    null_counter(integer.mantissa, strlen(integer.mantissa) - 1, &counter2, -1);
    if (integer.degree >= 0)
    {
        if (counter1 > integer.degree + 1)
            return -1;
    }
    else
    {
        if (counter2 < -integer.degree + 1)
            return -1;
    }

    return 0;
}

short_t check_expon(char *num, size_t cur_len, size_t len)
{
    if (cur_len == len)
        return 1;

    if (toupper(num[cur_len]) == 'E')
        return 0;

    return check_expon(num, ++cur_len, len);
}