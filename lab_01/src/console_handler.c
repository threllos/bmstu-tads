#include <stdio.h>
#include <string.h>
#include "console_handler.h"


void write_info()
{
    printf("\nПРАВИЛА ВВОДА:\n");
    printf("1. Оба числа вводятся обязательно со знаком (+ или -)\n");
    printf("2. Только действительное вводится в экспоненциальном или в обычном формате\n");
    printf("3. Порядок числа обязательно записывается со знаком (+ или -)\n");
    printf("4. В порядке должна быть записана хотя бы 1 цифра\n");
    printf("5. Мантиса действительного числа не больше 30 цифр\n");
    printf("6. Целое число содержит не больше 30 цифр\n");
    printf("7. Порядок содержит не больше 5 цифр\n\n");
}

void write_range()
{
    printf("%32d %28d %2d %3d\n", 1, 30, 1, 5);
    printf("%71s", "±|----------------------------|E±|---|\n");
}

void write_result(const bignum_t result)
{
    if (!strcmp("\0", result.mantissa))
        printf("Результат: %23s\n", "+0.0"); 
    else if (result.degree >= 0)
        printf("Результат: %20c0.%se+%d\n", result.sign, result.mantissa, result.degree);
    else
        printf("Результат: %20c0.%se%d\n", result.sign, result.mantissa, result.degree);
}
