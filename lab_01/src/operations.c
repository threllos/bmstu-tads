#include "operations.h"
#include "parse.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char tmp[MAX_MANTISSA * 2];

char int_to_char(int number)
{
    return number + '0';
}

short_t to_exp(char *raw_int)
{
    size_t index = strlen(raw_int);
    char symbols[3] = {'e', '+', '0'};
    for (size_t i = 0; i < 3; i++)
    {
        if (index > 50)
            return -1;

        raw_int[index++] = symbols[i];
    }
    return 0;
}

void normalization(bignum_t *number)
{
    size_t k = 0;
    size_t len = strlen(number->mantissa);
    size_t i = len - number->point_index;

    while (number->mantissa[i++] == '0' && i < len)
    {
        k += 1;
    }

    if (k == 0)
        return;

    number->degree -= k;

    for (size_t q = len - 1; q > k; q--)
    {
        number->mantissa[q - k] = number->mantissa[q];
        number->mantissa[q] = '\0';
    }

    number->point_index -= k;
}

void discharge_multiplication(const char *num, int len, char *temp, int tmp_len, const int result_num)
{
    int discharge_num = result_num * (num[--len] - '0');
    temp[tmp_len] = int_to_char(char_to_int(temp[tmp_len]) + discharge_num % 10);
    temp[tmp_len - 1] = int_to_char(char_to_int(temp[tmp_len - 1]) + discharge_num / 10);

    if (len == 0)
        return;

    discharge_multiplication(num, len, temp, tmp_len - 1, result_num);
}

void discharge_sum(char *result_number, const char *temp, int len)
{
    int discharge_num = char_to_int(temp[len]) + char_to_int(result_number[len]);

    result_number[len] = int_to_char(discharge_num % 10);
    result_number[len - 1] = int_to_char(char_to_int(result_number[len - 1]) + discharge_num / 10);

    if (len == 1)
        return;

    discharge_sum(result_number, temp, len - 1);
}


int mantissa_multiplication(bignum_t *int_number, bignum_t *float_number, char *result_mantissa)
{
    size_t len1 = strlen(int_number->mantissa), len2 = strlen(float_number->mantissa);
    int discharge = 1;
    size_t tmp_len = (MAX_MANTISSA * 2 - discharge);

    while (len1 != 0)
    {
        memset(tmp, '0', sizeof(tmp));
        discharge_multiplication(float_number->mantissa, len2, tmp, tmp_len,
                                 char_to_int(int_number->mantissa[len1 - 1]));
        discharge_sum(result_mantissa, tmp, (MAX_MANTISSA * 2) - 1);

        len1 -= 1;
        tmp_len -= 1;
        discharge += 1;
    }

    return 0;
}


void get_sign(const bignum_t num1, const bignum_t num2, bignum_t *result)
{
    if (num1.sign == num2.sign)
        result->sign = '+';
    else
        result->sign = '-';
}

void get_point_position(const bignum_t num1, const bignum_t num2, bignum_t *result)
{
    result->point_index = num1.point_index + num2.point_index;
}

void multiplication(bignum_t *int_number, bignum_t *float_number, bignum_t *result_number)
{
    mantissa_multiplication(int_number, float_number, result_number->mantissa);
    get_sign(*int_number, *float_number, result_number);
    get_point_position(*int_number, *float_number, result_number);
}

void null_round(bignum_t *result_number, size_t index)
{
    if (result_number->mantissa[index] == '0')
    {
        result_number->mantissa[index] = '\0';

        if (index != 0)
            null_round(result_number, index - 1);
    }
}

void rounding(bignum_t *number, size_t end_ind, short_t flag)
{
    size_t int_numb = number->mantissa[end_ind + 1] - '0';
    size_t int_numb_cur = number->mantissa[end_ind] - '0';
    if (flag)
        number->mantissa[end_ind] = int_to_char((int_numb_cur + 1) % 10);

    if (number->mantissa[end_ind + 1] >= '5')
    {
        number->mantissa[end_ind] = int_to_char((int_numb + 1) % 10);
        number->mantissa[end_ind + 1] = 0;
    }

    if (number->mantissa[end_ind + 1] == '0' && end_ind != 0)
        rounding(number, --end_ind, 1);
}

void rounding2(bignum_t *number, size_t end_ind, short_t flag)
{
    size_t int_numb_cur = number->mantissa[end_ind] - '0';

    if (flag)
        number->mantissa[end_ind] = int_to_char((int_numb_cur + 1) % 10);

    if (number->mantissa[end_ind + 1] >= '5')
        number->mantissa[end_ind] = int_to_char((int_numb_cur + 1) % 10);

    if (number->mantissa[end_ind] == '0' && end_ind != 0)
        rounding2(number, --end_ind, 1);
}

void move_to_left(char *mantissa)
{
    size_t len = strlen(mantissa);
    for (size_t i = 1; i < len; i++)
        mantissa[i - 1] = mantissa[i];
    mantissa[len - 1] = '\0';
}
