#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "operation.h"
#include "array_operations.h"

#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;36m"
#define BLACK   "\033[0;30;47m"

void flush_array(void)
{
    char c;

    while((scanf("%c", &c) == 1) && (c != '\n'))
        ;
}

int top_arr(array_t a)
{
    return *(a.arr - 1);
}


int check_clear_stack(array_t s)
{
    return (s.len == 0) ? 0 : 1;
}

void push_arr(array_t *a, int n)
{
    *(a->arr++) = n;
    a->len++;
}

int result_arr(int *r)
{
    exp_t exp;

    if (read_expression(&exp))
        return EXIT_FAILURE;
    
    *r = calculate_arr(exp);
 
    return EXIT_SUCCESS;
}

int create_arr(array_t *array, int *max_len)
{
    int *tmp;
    int cur_len = 0;
    int num;

    if (*max_len == 0)
    {
        printf("%sВведите размер стека (1..10000):%s ", BLUE, RESET);

        if ((scanf("%d", max_len) != 1) || (*max_len < 1) || (*max_len > 10000))
        {
            printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }

        tmp = malloc(*max_len * sizeof(int));

        if (!tmp)
            return EXIT_FAILURE;

        array->arr = tmp;
    }

    printf("%sСколько элементов нужно ввести:%s ", BLUE, RESET);

    if ((scanf("%d", &cur_len) != 1) || (cur_len < 1) || (array->len + cur_len > *max_len))
    {
        printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    printf("%sВведите элементы по одному:%s ", BLUE, RESET);

    for (size_t i = 0; i < cur_len; i++)
    {
        if (i > 0)
            printf("%28s", "");

        if (scanf("%d", &num) == 1)
            push_arr(array, num);
        else
        {
            printf("%sWar: некорректный ввод, повторите%s\n", YELLOW, RESET);
            flush_array();
            i--;
        }
    }

    printf("%sСтек массивом успешно создан%s\n", GREEN, RESET);
    printf("%sВерхний элемент стека -%s %d\n", GREEN, RESET, top_arr(*array));

    return EXIT_SUCCESS;
}

int print_arr(array_t a)
{
    int *tmp;
    size_t len;

    if (a.len == 0)
    {
        printf("%sWar: Стек пуст%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    tmp = a.arr;
    len = a.len;
    printf("%sСтек в виде массива%s\n", GREEN, RESET);

    while (len > 0)
    {
        printf("%d\n", *(--tmp));
        len--;
    }

    return EXIT_SUCCESS;
}

int pop_arr(array_t *a)
{
    if (a->len == 0)
    {
        printf("%sWar: Стек пуст%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    a->arr--;
    a->len--;

    return EXIT_SUCCESS;
}

int add_elem_arr(array_t *array, int *max_len)
{
    int *tmp;
    int num;

    if (*max_len == 0)
    {
        printf("%sВведите размер стека (1..10000):%s ", BLUE, RESET);

        if ((scanf("%d", max_len) != 1) || (*max_len < 0) || (*max_len > 10000))
        {
            printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }

        tmp = malloc(*max_len * sizeof(int));

        if (!tmp)
            return EXIT_FAILURE;

        array->arr = tmp;
    }

    if (array->len + 1 > *max_len)
    {
        printf("%sWar: стек переполнен%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    printf("%sВведите число:%s ", BLUE, RESET);

    if (scanf("%d", &num) != 1)
    {
        printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    push_arr(array, num);

    return EXIT_SUCCESS;
}

void count_operation(array_t *num_stack, array_t *sign_stack)
{
    int result;
    int num1, num2;
    int sign;

    num1 = top_arr(*num_stack);
    pop_arr(num_stack);
    num2 = top_arr(*num_stack);
    pop_arr(num_stack);
    sign = top_arr(*sign_stack);
    pop_arr(sign_stack);

    if (sign == 1)
        result = num2 + num1;
    else if (sign == 2)
        result = num2 - num1;
    else if (sign == 3)
        result = num2 * num1;
    else if (sign == 4)
        result = (int)(num2 / num1);

    push_arr(num_stack, result);
}

int calculate_arr(exp_t exp)
{
    int *tmp;

    array_t num_stack = {.len = 0};
    array_t sym_stack = {.len = 0};
    tmp = malloc(exp.len * sizeof(int));

    if (!tmp)
        return EXIT_FAILURE;

    num_stack.arr = tmp;
    tmp = malloc(exp.len * sizeof(int));

    if (!tmp)
        return EXIT_FAILURE;

    sym_stack.arr = tmp;
    
    for (size_t i = 0; i < exp.len; i++)
    {
        if (check_operation(exp.sym[i]))
        {
            int sign = translate_operation(exp.sym[i]);

            if (!check_clear_stack(sym_stack))
                push_arr(&sym_stack, sign);
            else
            {
                int level_cur = check_priority(sign);
                int level_prev = check_priority(top_arr(sym_stack));

                if (level_cur > level_prev)
                    push_arr(&sym_stack, sign);
                else if (level_cur <= level_prev)
                {
                    count_operation(&num_stack, &sym_stack);
                    i--;
                }
            }
        }
        else
        {
            int num = read_num_from_arr_char(exp.sym, &i);
            i -= 1;
            push_arr(&num_stack, num);

            if (i + 1 == exp.len)
                while (check_clear_stack(sym_stack))
                    count_operation(&num_stack, &sym_stack);
        }
    }

    return top_arr(num_stack);
}
