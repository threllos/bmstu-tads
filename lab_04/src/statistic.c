#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "list_operations.h"
#include "array_operations.h"
#include "operation.h"
#include "structs.h"
#include "statistic.h"

#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;36m"
#define BLACK   "\033[0;30;47m"

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__ (
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r" (high), "=r" (low)
        :: "%rax", "%rbx", "%rcx", "%rdx"
        );

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}

void print_time(int64_t start1, int64_t end1, int64_t start2, int64_t end2)
{
    printf("%sСтек массивом:%s %lld тактов\n", GREEN, RESET, end1 - start1);
    printf("%sСтек списком:%s  %lld тактов\n", GREEN, RESET, end2 - start2);
}

void create_rand_nums(int *arr, int count)
{
    srand(time(NULL));

    for (int i = 0; i < count; i++)
    {
        int num = rand() % 10;
        arr[i] = num;
    }
}

void time_add(int count)
{
    int64_t start1 = 0, end1 = 0, start2= 0, end2 = 0;

    list_t *stack_list = create_node(33);
    array_t stack_arr = {.arr = NULL, .len = 0};

    int *tmp = malloc(count * sizeof(int));

    if (!tmp)
        return;

    stack_arr.arr = tmp;
    int arr[1000];
    create_rand_nums(arr, count);

    for (int j = 0; j < RUNS; j++)
    {
        start1 += tick();
        for (int i = 0; i < count; i++)
        {
            push_arr(&stack_arr, arr[i]);
        }
        end1 += tick();

        start2 += tick();
        for (int i = 0; i < count; i++)
        {
            push_node(&stack_list, arr[i]);
        }
        end2 += tick();

        for (int i = 0; i < count; i++)
        {
            pop_arr(&stack_arr);
            pop_list(&stack_list);
        }
    }

    printf("%sЗамеры добавления элемента%s\n", GREEN, RESET);
    print_time(start1 / RUNS, end1 / RUNS, start2 / RUNS, end2 / RUNS);
}

void time_del(int count)
{
    int64_t start1 = 0, end1 = 0, start2 = 0, end2 = 0;

    list_t *stack_list = create_node(33);
    array_t stack_arr = {.arr = NULL, .len = 0};

    int *tmp = malloc(count * sizeof(int));

    if (!tmp)
        return;

    stack_arr.arr = tmp;
    int arr[1000];
    create_rand_nums(arr, count);

    for (int j = 0; j < RUNS; j++)
    {
        for (int i = 0; i < count; i++)
        {
            push_arr(&stack_arr, arr[i]);
            push_node(&stack_list, arr[i]);
        }

        start1 += tick();
        for (int i = 0; i < count; i++)
        {
            pop_arr(&stack_arr);
        }
        end1 += tick();

        start2 += tick();
        for (int i = 0; i < count; i++)
        {
            pop_list(&stack_list);
        }
        end2 += tick();
    }

    printf("%sЗамеры удаления элемента%s\n", GREEN, RESET);
    print_time(start1 / RUNS, end1 / RUNS, start2 / RUNS, end2 / RUNS);
}

void time_process(int count)
{
    int64_t start1 = 0, end1 = 0, start2 = 0, end2 = 0;
    int arr[1000];
    create_rand_nums(arr, count);
    exp_t expression;
    int flag = 0;

    for (int i = 0; i < count; i++)
    {
        if (!flag)
        {
            expression.sym[i] = arr[i] + '0';
            flag = 1;
        }
        else
        {
            expression.sym[i] = '+';
            flag = 0;
        }
    }

    expression.sym[count - 1] = '1';
    expression.sym[count] = '\0';
    expression.len = count;

    for (int i = 0; i < RUNS; i++)
    {
        start1 += tick();
        calculate_arr(expression);
        end1 += tick();
        start2 += tick();
        calculate_list(expression);
        end2 += tick();
    }

    printf("%sЗамеры вычисления выражения%s\n", GREEN, RESET);
    print_time(start1 / RUNS, end1 / RUNS, start2 / RUNS, end2 / RUNS);
}

void time_counting(void)
{
    printf("%sВведите размер стека (1..10000):%s ", BLUE, RESET);
    int count;

    if ((scanf("%d", &count) != 1) || (count < 1) || (count > 1000))
    {
        printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
        return;
    }

    printf("Тестирование для %s%d%s элементов\n", GREEN, count, RESET);

    time_add(count);
    time_del(count);
    time_process(count);

    printf("Замеры памяти для %s%d%s элементов:\n", GREEN, count, RESET);
    printf("%sПод массив:%s %lld байт\n", GREEN, RESET, count * sizeof(int) + sizeof(int*));
    printf("%sПод список:%s %lld байт\n", GREEN, RESET, count * sizeof(list_t));
}