#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "operation.h"
#include "list_operations.h"

#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;36m"
#define BLACK   "\033[0;30;47m"

void flush_list(void)
{
    char c;

    while((scanf("%c", &c) == 1) && (c != '\n'))
        ;
}

int check_clear_stack_list(list_t *l)
{
    return (!l) ? 0 : 1;
}

void print_arr_adress(arr_clear_t a)
{
    size_t len;

    if (a.len == 0)
    {
        printf("%sWar: Массив освободившихся адрессов пуст%s\n", YELLOW, RESET);
        return;
    }

    len = a.len;
    printf("%sМассив освободившихся адрессов%s\n", GREEN, RESET);

    for (size_t i = 0; i < len; i++)
        printf("%p\n", (void*)a.arr_clear[i]);
    printf("\n");
}

list_t* create_node(int num)
{
    list_t *tmp_elem = malloc(sizeof(list_t));

    if (!tmp_elem)
        return NULL;

    tmp_elem->num = num;
    tmp_elem->ind = 0;
    tmp_elem->next = NULL;

    return tmp_elem;
}

int push_node(list_t **list, int num)
{
    list_t *node = create_node(num);

    if (!node)
        return EXIT_FAILURE;

    node->ind = (*list)->ind + 1;
    node->next = *list;
    
    *list = node;

    return EXIT_SUCCESS;
}

int pop_list(list_t **list)
{
    if (!(*list))
    {
        printf("%sWar: Стек пуст%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    list_t *tmp = (*list)->next;
    free(*list);
    *list = tmp;

    return EXIT_SUCCESS;
}

void free_list(list_t **l)
{
    size_t len = (*l)->ind;

    while (len >= 0)
    {
        pop_list(l);
        len--;
    }
}

int add_elem_list(list_t **list, int *max_len)
{
    if (*max_len == 0)
    {
        printf("%sВведите размер стека (1..10000):%s ", BLUE, RESET);

        if ((scanf("%d", max_len) != 1) || (*max_len < 0) || (*max_len > 10000))
        {
            printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }
    }

    if (*list)
    {
        if ((*list)->ind + 2 > *max_len)
        {
            printf("%sWar: стек переполнен%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }
    }

    int num;
    printf("%sВведите число:%s ", BLUE, RESET);

    if ((scanf("%d", &num) != 1))
    {
        printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    if (!(*list))
    {
        *list = create_node(num);
    }
    else
    {
        if (push_node(list, num))
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int result_list(int *r)
{
    exp_t exp;

    if (read_expression(&exp))
        return EXIT_FAILURE;

    *r = calculate_list(exp);

    return EXIT_SUCCESS;
}

int create_list(list_t **stack_list, int *max_len)
{   
    int num;
    int cur_len = 0;

    if (*max_len == 0)
    {
        printf("%sВведите размер стека (1..10000):%s ", BLUE, RESET);

        if ((scanf("%d", max_len) != 1) || (*max_len < 0) || (*max_len > 10000))
        {
            printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }
    }

    printf("%sСколько элементов нужно ввести:%s ", BLUE, RESET);

    if ((scanf("%d", &cur_len) != 1) || (cur_len < 1) || (cur_len > *max_len))
    {
        printf("%sWar: недопустимое значение%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    printf("%sВведите элементы по одному:%s ", BLUE, RESET);

    for (size_t i = 0; i < cur_len; i++)
    {   
        if (i > 0)
            printf("%28s", "");

        if (scanf("%d", &num) != 1)
        {
            printf("%sWar: некорректный ввод, повторите%s\n", YELLOW, RESET);
            flush_list();
            i--;
        }
        else
        {
            if (!(*stack_list))
                *stack_list = create_node(num);
            else
                if (push_node(stack_list, num))
                    return EXIT_FAILURE;
        }
    }

    printf("%sСтек списком успешно создан%s\n", GREEN, RESET);

    return EXIT_SUCCESS;
}

int top_list(list_t *l)
{
    return (l->num);
}

int print_list(list_t *list)
{
    int len;

    if (!list)
    {
        printf("%sWar: Стек пуст%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    len = list->ind;
    printf("%sСтек в виде списка%s\n", GREEN, RESET);
    list_t *tmp = list;

    while (len >= 0)
    {
        printf("%d - %p\n", tmp->num, (void*)tmp);
        tmp = tmp->next;

        len--;
    }

    return EXIT_SUCCESS;
}

void count_operation_list(list_t **num_stack, list_t **sign_stack)
{
    int result;
    int num1, num2;
    int sign;

    num1 = top_list(*num_stack);
    pop_list(num_stack);
    num2 = top_list(*num_stack);
    pop_list(num_stack);
    sign = top_list(*sign_stack);
    pop_list(sign_stack);

    if (sign == 1)
        result = num2 + num1;
    else if (sign == 2)
        result = num2 - num1;
    else if (sign == 3)
        result = num2 * num1;
    else if (sign == 4)
        result = (int)(num2 / num1);

    if (!check_clear_stack_list(*num_stack))
        *num_stack = create_node(result);
    else
        push_node(num_stack, result);
}

int calculate_list(exp_t exp)
{
    list_t *num_stack = NULL;
    list_t *sym_stack = NULL;

    for (size_t i = 0; i < exp.len; i++)
    {
        if (check_operation(exp.sym[i]))
        {
            int sign = translate_operation(exp.sym[i]);

            if (!check_clear_stack_list(sym_stack))
                sym_stack = create_node(sign);
            else
            {
                int level_cur = check_priority(sign);
                int level_prev = check_priority(top_list(sym_stack));

                if (level_cur > level_prev)
                    push_node(&sym_stack, sign);
                else if (level_cur <= level_prev)
                {
                    count_operation_list(&num_stack, &sym_stack);
                    i--;
                }
            }
        }
        else
        {
            int num = read_num_from_arr_char(exp.sym, &i);
            i -= 1;

            if (!check_clear_stack_list(num_stack))
                num_stack = create_node(num);
            else
                push_node(&num_stack, num);

            if (i + 1 == exp.len)
                while (check_clear_stack_list(sym_stack))
                    count_operation_list(&num_stack, &sym_stack);
        }
    }    

    return top_list(num_stack);
}

void add_adress(arr_clear_t *arr, list_t *list)
{
    if (!list)
        return;

    arr->arr_clear[arr->len] = list;
    arr->len += 1;
}

void adress_clear(arr_clear_t *a, int ind)
{
    for (int i = ind; i < a->len - 1; i++)
    {
        list_t *tmp = a->arr_clear[i];
        a->arr_clear[i] = a->arr_clear[i + 1];
        a->arr_clear[i + 1] = tmp;
    }
}

void del_adress(arr_clear_t *a, list_t *list)
{
    if (!list)
        return;

    list_t *tmp = list;
    int len = tmp->ind;

    for (int i = 0; i < a->len; i++)
    {
        tmp = list;

        for (int j = len; j >= 0; j--)
        {
            if (a->arr_clear[i] == tmp)
            {
                adress_clear(a, i);
                a->len -= 1;
                i--;
            }
            
            tmp = tmp->next;
        }
    }
}
