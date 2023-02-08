#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "list_operations.h"
#include "array_operations.h"
#include "operation.h"
#include "statistic.h"

#define COMMANDS 13

#define CMD_PROGRAMM_EXIT       0
#define CMD_LIST_CREATE         1
#define CMD_LIST_ADD            2
#define CMD_LIST_DELETE         3
#define CMD_LIST_PRINT          4
#define CMD_LIST_CALC           5
#define CMD_LIST_FREE_ADRESS    6
#define CMD_ARRAY_CREATE        7
#define CMD_ARRAY_ADD           8
#define CMD_ARRAY_DELETE        9
#define CMD_ARRAY_PRINT         10
#define CMD_ARRAY_CALC          11
#define CMD_STATS               12

#define ERROR_COMMAND           100
#define ERROR_CASE              101
#define ERROR_CREATE_LIST       200

#define RESET                   "\033[0m"
#define GREEN                   "\033[0;32m"
#define RED                     "\033[0;31m"
#define YELLOW                  "\033[0;33m"
#define BLUE                    "\033[0;36m"
#define BLACK                   "\033[0;30;47m"

void flush_input(void)
{
    char c;
    while((scanf("%c", &c) == 1) && (c != '\n'))
        ;
}

void print_menu(void)
{
        printf("\n\t%sКоманды%s\n", GREEN, RESET);
        printf(" 0. Выйти из програмы\n");
        printf("\t%sСтек как список%s\n", GREEN, RESET);
        printf(" 1. Заполнить стек\n");
        printf(" 2. Добавить элемент в стек\n");
        printf(" 3. Удалить элемент из стека\n");
        printf(" 4. Текущее состояние стека\n");
        printf(" 5. Вычислить с помощью стека\n");
        printf(" 6. Свободные адресса стека\n");
        printf("\t%sСтек как массив%s\n", GREEN, RESET);
        printf(" 7. Заполнить стек\n");
        printf(" 8. Добавить элемент в стек\n");
        printf(" 9. Удалить элемент из стека\n");
        printf("10. Текущее состояние стека\n");
        printf("11. Вычислить с помощью стека\n");
        printf("\t%sСтатистика%s\n", GREEN, RESET);
        printf("12. Статистика по времени и памяти\n");

        printf("%sВведите команду:%s ", BLUE, RESET);
}

int main(void)
{
    setbuf(stdout, NULL);

    int max_len_list = 0, max_len_arr = 0;

    list_t *stack_list = NULL;

    array_t stack_arr = {.arr = NULL, .len = 0};
    arr_clear_t clear_arr = {.len = 0};

    int command = -1;
    int flag = 1;
    int result;

    while (flag == 1)
    {
        print_menu();

        if ((scanf("%d", &command) != 1) || (command < 0) || (command > COMMANDS))
        {
            printf("%sErr: пункты меню это числа от 0 до 13%s\n", RED, RESET);
            return ERROR_COMMAND;
        }

        switch(command)
        {
            case CMD_PROGRAMM_EXIT:
                flag = 0;

                break;
            case CMD_LIST_CREATE:
                if (create_list(&stack_list, &max_len_list))
                {
                    flush_input();
                    break;
                }

                del_adress(&clear_arr, stack_list);

                flush_input();
                break;
            case CMD_LIST_ADD:
                if (!add_elem_list(&stack_list, &max_len_list))
                {
                    printf("%sЭлемент успешно добавлен%s\n", GREEN, RESET);
                    del_adress(&clear_arr, stack_list);
                }

                flush_input();
                break;
            case CMD_LIST_DELETE:
                add_adress(&clear_arr, stack_list);

                if (!pop_list(&stack_list))
                    printf("%sЭлемент удален%s\n", GREEN, RESET);

                flush_input();
                break;
            case CMD_LIST_PRINT:
                print_list(stack_list);

                flush_input();
                break;
            case CMD_LIST_CALC:
                if(!result_list(&result))
                    printf("%sРезультат вычисления выражения:%s %d\n", GREEN, RESET, result);

                break;
            case CMD_LIST_FREE_ADRESS:
                print_arr_adress(clear_arr);

                flush_input();
                break;
            case CMD_ARRAY_CREATE:
                create_arr(&stack_arr, &max_len_arr);

                flush_input();
                break;
            case CMD_ARRAY_ADD:
                if (!add_elem_arr(&stack_arr, &max_len_arr))
                    printf("%sЭлемент успешно добавлен%s\n", GREEN, RESET);

                flush_input();
                break;
            case CMD_ARRAY_DELETE:
                if (!pop_arr(&stack_arr))
                    printf("%sЭлемент удален%s\n", GREEN, RESET);

                flush_input();
                break;
            case CMD_ARRAY_PRINT:
                print_arr(stack_arr);

                flush_input();
                break;
            case CMD_ARRAY_CALC:
                if(!result_arr(&result))
                    printf("%sРезультат вычисления выражения:%s %d\n", GREEN, RESET, result);

                break;
            case CMD_STATS:
                time_counting();

                flush_input();
                break;
            default:
                printf("%sЧто-то пошло не так...%s\n", RED, RESET);
                return ERROR_CASE;
        }
    }
    
    return EXIT_SUCCESS;
}