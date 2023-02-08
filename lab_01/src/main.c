#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"
#include "errors.h"
#include "check.h"
#include "parse.h"
#include "operations.h"
#include "console_handler.h"


int main()
{
    setbuf(stdout, NULL);
    

// Инициализация переменных

    char integer_raw[MAX_RAW_LEN] = {'\0'}, float_raw[MAX_RAW_LEN] = {'\0'};
    bignum_t integer_n, float_n, result_n;
    memset(result_n.mantissa, '0', sizeof(result_n.mantissa));
    short_t fl = 0;
    short_t rc = 0;
    write_info();

// Обработка действительного числа
    // Ввод действительного числа
    write_range();
    printf("Введите действительное число: ");
    if (scanf("%50s", float_raw) != 1)
    {
        printf("Ошибка ввода действительного числа\n");
        return FLOAT_INPUT_ERROR;
    }

    // Обработка ошибок ввода
    rc = input_check(float_raw);
    if (rc != 0)
    {
        /*
        if (rc == INPUT_FORBIDDEN_SYMBOL)
            printf("Ошибка: недопустимые символы при вводе\n");
        else if (rc == INPUT_E_ERROR)
            printf("Ошибка: избыточное количество символов Е\n");
        else if (rc == INPUT_SIGN_ERROR)
            printf("Ошибка в количестве знаков\n");
        else if (rc == INPUT_DOTS_IN_DEGREE)
            printf("Ошибка: точка в порядке\n");
        else if (rc == INPUT_DOTS_IN_MANTISSA)
            printf("Ошибка: избыточное количество точек в мантиссе\n");
        */
        printf("Ошибка ввода действительного числа\n");
        return rc;
    }

    // Приведение числа к эспоненциальному виду
    if (check_expon(float_raw, 0, strlen(float_raw)))
    {
        rc = to_exp(float_raw);
        if (rc == -1)
        {
            printf("Ошибка перевода в экспоненциальный вид\n");
            return INTEGER_TO_EXPON_ERROR;
        }
    }

    // Конвертация строки в действительное число
    rc = parse(float_raw, &float_n);
    if (rc == PARSING_MANTISSA_ERROR)
    {
        printf("Ошибка: мантисса должна быть не больше 30 символов\n");
        return rc;
    }
    if (rc == PARSING_DEGREE_OUT_RANGE)
    {
        printf("Ошибка: порядок должен быть не больше 5 символов\n");
        return rc;
    }

    // Проверка на действительную мантиссу
    if (result_of_mantissa_parsing_check(float_n.mantissa))
    {
        printf("Ошибка ввода действительного числа\n");
        return FLOAT_DATA_INVALID;
    }

// Обработка целого числа
    // Ввод целого числа
    printf("Введите целое число: %9s", " ");
    if (scanf("%50s", integer_raw) != 1)
    {
        printf("Ошибка целого числа\n");
        return INTEGER_INPUT_ERROR;
    }

    // Обработка ошибок ввода
    rc = integer_check(integer_raw);
    if (rc == 1)
    {
        printf("Ошибка ввода целого числа\n");
        return rc;
    }

    // Приведение числа к эспоненциальному виду
    if (check_expon(integer_raw, 0, strlen(integer_raw)))
    {
        rc = to_exp(integer_raw);
        if (rc == -1)
        {
            printf("Ошибка перевода в экспоненциальный вид\n");
            return INTEGER_TO_EXPON_ERROR;
        }
    }

    // Конвертация строки в действительное число
    rc = parse(integer_raw, &integer_n);
    if (rc == PARSING_MANTISSA_ERROR)
    {
        printf("Ошибка: мантисса должна быть не больше 30 символов\n");
        return rc;
    }
    if (rc == PARSING_DEGREE_OUT_RANGE)
    {
        printf("Ошибка: порядок должен быть не больше 5 символов\n");
        return rc;
    }

    // Обработка формата целого числа
    rc = integer_e_check(integer_n);
    if (rc == -1)
    {
        printf("Ошибка формата ввода целого числа\n");
        return INTEGER_WRONG_FORMAT;
    }


// Нормализация чисел

    if (float_raw[1] == '0' && float_raw[2] == '.')
    {
        fl = 1;
        normalization(&float_n);
    }
    if (integer_raw[1] == '0' && integer_raw[2] == '.')
    {
        fl = 1;
        normalization(&integer_n);
    }

// Умножение чисел

    multiplication(&integer_n, &float_n, &result_n);

// Обработка результата

    rc = post_process(integer_n, float_n, &result_n, fl);
    if (rc == PARSING_OVERFLOW)
    {
        printf("Предупреждение: мантисса была округлена\n");

        rounding2(&result_n, MAX_MANTISSA - 2, 0);
        result_n.mantissa[MAX_MANTISSA - 1] = '\0';
    }
    if (rc == PARSING_RANGE_ERROR)
    {
        printf("Ошибка переполнения порядка\n");
        return PARSING_OVERFLOW;
    }

    null_round(&result_n, strlen(result_n.mantissa) - 1);

// Вывод результата

    write_result(result_n);

    return EXIT_SUCCESS;
}
