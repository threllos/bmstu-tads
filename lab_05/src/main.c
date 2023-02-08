#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "service.h"
#include "array.h"
#include "list.h"

#define ARRAY 1
#define LIST 2
#define ITERATIONS 1000

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__(
    "rdtsc\n"
    "movl %%edx, %0\n"
    "movl %%eax, %1\n"
    : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}

int main()
{
    setbuf(stdout, NULL);
    printf("size - %lld\n", sizeof(double));
    int command;
    int t1_min = T1_MIN, t1_max = T1_MAX;
    int t2_min = T2_MIN, t2_max = T2_MAX;
    int t3_min = T3_MIN, t3_max = T3_MAX;
    int t4_min = T4_MIN, t4_max = T4_MAX;

    printf("Моделирование очереди.\n");

    if (change_time(&t1_min, &t1_max,
                &t2_min, &t2_max,
                &t3_min, &t3_max,
                &t4_min, &t4_max))
        return EXIT_FAILURE;

    print_menu();

    if (scanf("%d", &command) == 1)
    {
        switch (command)
        {
            case 1:
            {
                int info;
                int info_print;
                uint64_t start, end;

                queue_arr_t arr_1, arr_2;
                init_arr(&arr_1);
                init_arr(&arr_2);

                print_addresses();

                if (scanf("%d", &info) != 1)
                    return EXIT_FAILURE;
                else if (info == 1)
                    info_print = 0;
                else if (info == 2)
                    info_print = 1;
                else
                {
                    printf("Неверный пункт меню\n");
                    return EXIT_FAILURE;
                }

                start = tick();
                process((void *) &arr_1, (void *) &arr_2, ARRAY, info_print,
                        t1_min, t1_max, t2_min, t2_max,
                        t3_min, t3_max, t4_min, t4_max);
                end = tick();

                printf("\nВремя выполнения программы:\t");
                printf("%lld - тиков.\n", end - start);
                break;
            }
            case 2:
            {
                int info;
                int info_print;
                uint64_t start, end;

                queue_list_t list_1, list_2;
                init_list(&list_1);
                init_list(&list_2);

                print_addresses();

                if (scanf("%d", &info) != 1)
                    return EXIT_FAILURE;
                else if (info == 1)
                    info_print = 0;
                else if (info == 2)
                    info_print = 1;
                else
                {
                    printf("Неверный пункт меню\n");
                    return EXIT_FAILURE;
                }

                start = tick();
                process((void *) &list_1, (void *) &list_2, LIST, info_print,
                        t1_min, t1_max, t2_min, t2_max,
                        t3_min, t3_max, t4_min, t4_max);
                end = tick();

                printf("\nВремя выполнения программы:\t");
                printf("%lld - тиков.\n", end - start);

                free_list(*list_1.head);
                free_list(*list_2.head);
                break;
            }
            case 3:
            {
                queue_arr_t arr;
                queue_list_t list;
                init_arr(&arr);
                init_list(&list);
                uint64_t start, end;

                printf("\n");
                printf("Размер массива: %zu\n",
                       sizeof(arr));
                printf("Размер списка : %zu\n",
                       sizeof(list) + 2999 * (sizeof(node_t)));

                start = tick();
                for (size_t i = 0; i < ITERATIONS; i++)
                    push_arr(&arr, (float) i);
                end = tick();
                printf("Массив push   : %llu\n", (end - start) / ITERATIONS);

                start = tick();
                for (size_t i = 0; i < ITERATIONS; i++)
                    push_list(&list, (float) i);
                end = tick();
                printf("Список push   : %llu\n", (end - start) / ITERATIONS);

                start = tick();
                for (size_t i = 0; i < ITERATIONS; i++)
                    pop_arr(&arr);
                end = tick();
                printf("Массив pop    : %llu\n", (end - start) / ITERATIONS);

                start = tick();
                for (size_t i = 0; i < ITERATIONS; i++)
                    pop_list(&list);
                end = tick();
                printf("Список pop    : %llu\n", (end - start) / ITERATIONS);

                free_list(*list.head);

                break;
            }
            case 0:
                printf("Выход из программы\n");
                return EXIT_SUCCESS;
            default:
                printf("Неверный пункт меню\n");
                return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Неверный пункт меню\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
