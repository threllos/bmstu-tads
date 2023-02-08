#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "service.h"
#include "array.h"
#include "list.h"

void print_time(void)
{
    printf("\nХотите ли вы поменять стандартные настройки времени?\n");
    printf("\t1. Нет\n");
    printf("\t2. Да\n");
    printf("Введите пункт: "); 
}

void print_menu(void)
{
    printf("\nМеню:\n");
    printf("\t1. Массив\n");
    printf("\t2. Список\n");
    printf("\t3. Тестирование\n");
    printf("\t0. Выход\n");
    printf("Введите пункт: ");
}

void print_addresses(void)
{
    printf("\nОтображать свободные адресса?\n");
    printf("\t1. Нет\n");
    printf("\t2. Да\n");
    printf("Введите пункт: ");
}

int change_time(int *t1_min, int *t1_max, int *t2_min, int *t2_max,
                int *t3_min, int *t3_max, int *t4_min, int *t4_max)
{
    int flag;

    print_time();

    if (scanf("%d", &flag) != 1)
        return EXIT_FAILURE;

    if (flag == 1)
        return EXIT_SUCCESS;
    else if (flag == 2)
    {
        printf("Введите T1 (min - max): ");
        scanf("%d%d", t1_min, t1_max);
        if (*t1_min < 0 || *t1_max < 0 || *t1_max < *t1_min)
        {
            printf("Введено неверное время\n");
            return EXIT_FAILURE;
        }

        printf("Введите T2 (min - max): ");
        scanf("%d%d", t2_min, t2_max);
        if (*t2_min < 0 || *t2_max < 0 || *t2_max < *t2_min)
        {
            printf("Введено неверное время\n");
            return EXIT_FAILURE;
        }

        printf("Введите T3 (min - max): ");
        scanf("%d%d", t3_min, t3_max);
        if (*t3_min < 0 || *t3_max < 0 || *t3_max < *t3_min)
        {
            printf("Введено неверное время\n");
            return EXIT_FAILURE;
        }

        printf("Введите T4 (min - max): ");
        scanf("%d%d", t4_min, t4_max);
        if (*t4_min < 0 || *t4_max < 0 || *t4_max < *t4_min)
        {
            printf("Введено неверное время\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Введен неверный пункт\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

float my_rand(float t1, float t2)
{
    float scale = rand() / (float) RAND_MAX;
    return scale * (t2 - t1) + t1;
}


void init_service(service_t *service)
{
    service->passed_1 = 0;
    service->passed = 0;
    service->passed_2 = 0;
    service->timer = 0.0;
    service->total = 0.0;
    service->process_time = -1.0;
    service->len_2 = 0;
    service->status = 0;
    service->count_len = 0;
    service->len_1 = 0;
}

void runner(float *a, float *b, service_t *service)
{
    if (service->process_time > 0)
    {
        float min = *a;

        if (*b < min)
            min = *b;

        if (service->process_time < min)
            min = service->process_time;

        *a -= min;
        *b -= min;

        service->process_time -= min;
        service->timer += min;
    }
    else
    {
        if (*a < *b)
        {
            service->timer += *a;

            *b -= *a;
            *a -= *a;
        }
        else
        {
            service->timer += *b;

            *a -= *b;
            *b -= *b;
        }
    }
}

void added_len(service_t *service, int len_1, int len_2)
{
    service->count_len += 1;
    service->len_2 += len_2;
    service->len_1 += len_1;
}

float average(service_t service, int flag)
{
    if (flag == 0)
        return ((float) service.len_1 / (float) service.count_len);
    else if (flag == 1)
        return ((float) service.len_2 / (float) service.count_len);
    else
        return -1;
}

int process(void *data_1, void *data_2, int mode, int adresses_print_flag,
            int t1_min, int t1_max, int t2_min, int t2_max,
            int t3_min, int t3_max, int t4_min, int t4_max)
{
    srand(time(NULL));

    if (mode == 1)
    {
        printf("Mode array\n");

        int rule;

        service_t service;
        init_service(&service);

        queue_arr_t *queue_1 = (queue_arr_t *) data_1;
        queue_arr_t *queue_2 = (queue_arr_t *) data_2;

        queue_1->time_wait = my_rand(t1_min, t1_max);
        queue_2->time_wait = my_rand(t2_min, t2_max);

        if (queue_1->time_wait > queue_2->time_wait)
            rule = 1;
        else
            rule = 0;

        while (service.passed_1 < Q1_PASSED)
        {
            runner(&queue_1->time_wait, &queue_2->time_wait, &service);

            if (service.passed > Q1_PASSED && service.passed_1 == 0)
            {
                printf("Очередь зациклилась\n");
                return EXIT_FAILURE;
            }

            if (queue_1->time_wait == 0)
            {
                queue_1->queue_len += 1;
                float total = my_rand(t3_min, t3_max);

                if (push_arr(queue_1, total))
                    return EXIT_FAILURE;

                added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);

                if (adresses_print_flag == 1)
                    printf("Добавлено в очередь    1: %p\n", (void *) &queue_1->queue[queue_1->end - 1]);

                queue_1->time_wait = my_rand(t1_min, t1_max);
            }

            if (queue_2->time_wait == 0)
            {
                queue_2->queue_len += 1;
                float total = my_rand(t4_min, t4_max);

                if (push_arr(queue_2, total))
                    return EXIT_FAILURE;

                added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);

                if (adresses_print_flag == 1)
                    printf("Добавлено в очередь    2: %p\n", (void *) &queue_2->queue[queue_2->end - 1]);

                queue_2->time_wait = my_rand(t2_min, t2_max);
            }

            if (service.process_time == 0)
                service.status = 0;

            if (!service.status)
            {
                if (rule == 0)
                {
                    if ((queue_1->begin != queue_1->end))
                    {
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 1: %p\n", (void *) &queue_1->queue[queue_1->begin]);

                        service.process_time = pop_arr(queue_1);
                        service.total += service.process_time;

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_1 += 1;
                        service.passed += 1;
                    }
                    else if (queue_2->begin != queue_2->end)
                    {
                        rule = 1;
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 2: %p\n", (void *) &queue_2->queue[queue_2->begin]);

                        service.process_time = pop_arr(queue_2);

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_2 += 1;
                        service.passed += 1;
                    }
                    else
                        service.process_time = -1;
                }
                else
                {
                    if ((queue_2->begin != queue_2->end))
                    {
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 2: %p\n", (void *) &queue_2->queue[queue_2->begin]);

                        service.process_time = pop_arr(queue_2);

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_2 += 1;
                        service.passed += 1;
                    }
                    else if (queue_1->begin != queue_1->end)
                    {
                        rule = 0;
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 1: %p\n", (void *) &queue_1->queue[queue_1->begin]);

                        service.process_time = pop_arr(queue_1);
                        service.total += service.process_time;

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_1 += 1;
                        service.passed += 1;
                    }
                    else
                        service.process_time = -1;
                }
            }

            if (service.passed_1 > 0 && service.passed_1 % 100 == 0)
            {
                printf("\n");
                printf("Обработано заявок: %d\n", service.passed);
                printf("Обработано заявок 1го типа: %d\n", service.passed_1);
                printf("Текущая очередь 1го типа: %d\n", queue_1->end - queue_1->begin);
                printf("Текущая очередь 2го типа: %d\n", queue_2->end - queue_2->begin);
                printf("Средняя длина очереди 1го типа: %lf\n", average(service, 0));
                printf("Средняя длина очереди 2го типа: %lf\n", average(service, 1));
            }
        }

        float queue_1_in = (t1_min + t1_max) * 0.5;
        float queue_1_out = (t3_min + t3_max) * 0.5;
        float timer_modeling;

        if (queue_1_in > queue_1_out)
            timer_modeling = queue_1_in * Q1_PASSED;
        else
            timer_modeling = queue_1_out * Q1_PASSED;

        printf("\n");
        printf("\t\t-- РЕЗУЛЬТАТ --\n");
        printf("Общее время симуляции:\t\t%lf\n", service.timer);
        printf("Рассчитаное время симуляции:\t%lf\n", timer_modeling);
        printf("Погрешность:\t\t\t%lf%%\n\n", 100 * fabs(service.timer - timer_modeling) / service.timer);

        if (queue_1_in > queue_1_out)
        {
            float timer_waiting = timer_modeling - queue_1_out * Q1_PASSED;
            float service_waiting = service.timer - service.total;

            printf("Время простоя:\t\t\t%lf\n", service_waiting);
            printf("Рассчитанное время простоя:\t%lf\n", timer_waiting);
            printf("Погрешность:\t\t\t%lf%%\n\n", 100 * fabs(service_waiting - timer_waiting) / service_waiting);
        }
        else
            printf("Время простоя:\t\t\t0\n\n");

        printf("Заявок 1го типа поступило:\t%d\n", queue_1->queue_len);
        printf("Заявок 1го типа обработано:\t%d\n", service.passed_1);
        printf("Заявок 2го типа поступило:\t%d\n", queue_2->queue_len);
        printf("Заявок 2го типа обработано:\t%d\n", service.passed_2);
    }
    else
    {
        printf("Mode list\n");

        int rule;

        service_t service;
        init_service(&service);

        queue_list_t *queue_1 = (queue_list_t *) data_1;
        queue_list_t *queue_2 = (queue_list_t *) data_2;

        queue_1->time_wait = my_rand(t1_min, t1_max);
        queue_2->time_wait = my_rand(t2_min, t2_max);

        if (queue_1->time_wait > queue_2->time_wait)
            rule = 1;
        else
            rule = 0;

        while (service.passed_1 < Q1_PASSED)
        {
            runner(&queue_1->time_wait, &queue_2->time_wait, &service);

            if (service.passed > Q1_PASSED && service.passed_1 == 0)
            {
                printf("Очередь зациклилась\n");
                return EXIT_FAILURE;
            }

            if (queue_1->time_wait == 0)
            {
                queue_1->queue_len += 1;
                float total = my_rand(t3_min, t3_max);

                if (push_list(queue_1, total))
                    return EXIT_FAILURE;

                added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);

                if (adresses_print_flag == 1)
                {
                    node_t *node = *queue_1->head;

                    while (node->next != NULL)
                        node = node->next;

                    printf("Добавлено в очередь    1: %p\n", (void *) node);
                }

                queue_1->time_wait = my_rand(t1_min, t1_max);
            }

            if (queue_2->time_wait == 0)
            {
                queue_2->queue_len += 1;
                float total = my_rand(t4_min, t4_max);

                if (push_list(queue_2, total))
                    return EXIT_FAILURE;

                added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);

                if (adresses_print_flag == 1)
                {
                    node_t *node = *queue_2->head;

                    while (node->next != NULL)
                        node = node->next;

                    printf("Добавлено в очередь    2: %p\n", (void *) node);
                }

                queue_2->time_wait = my_rand(t2_min, t2_max);
            }

            if (service.process_time == 0)
                service.status = 0;

            if (!service.status)
            {
                if (rule == 0)
                {
                    if ((queue_1->begin != queue_1->end))
                    {
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 1: %p\n", (void *) *queue_1->head);

                        service.process_time = pop_list(queue_1);
                        service.total += service.process_time;

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_1 += 1;
                        service.passed += 1;
                    }
                    else if (queue_2->begin != queue_2->end)
                    {
                        rule = 1;
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 2: %p\n", (void *) *queue_2->head);

                        service.process_time = pop_list(queue_2);

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_2 += 1;
                        service.passed += 1;
                    }
                    else
                        service.process_time = -1;
                }
                else
                {
                    if ((queue_2->begin != queue_2->end))
                    {
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 2: %p\n", (void *) *queue_2->head);

                        service.process_time = pop_list(queue_2);

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_2 += 1;
                        service.passed += 1;
                    }
                    else if (queue_1->begin != queue_1->end)
                    {
                        rule = 0;
                        if (adresses_print_flag == 1)
                            printf("Освобождено из очереди 1: %p\n", (void *) *queue_1->head);

                        service.process_time = pop_list(queue_1);
                        service.total += service.process_time;

                        if (service.process_time < 0)
                            return EXIT_FAILURE;

                        added_len(&service, queue_1->end - queue_1->begin, queue_2->end - queue_2->begin);
                        service.status = 1;
                        service.passed_1 += 1;
                        service.passed += 1;
                    }
                    else
                        service.process_time = -1;
                }
            }

            if (service.passed_1 % 100 == 0 && service.passed_1 > 0)
            {
                printf("\n");
                printf("Обработано заявок: %d\n", service.passed);
                printf("Обработано заявок 1го типа: %d\n", service.passed_1);
                printf("Текущая очередь 1го типа: %d\n", queue_1->end - queue_1->begin);
                printf("Текущая очередь 2го типа: %d\n", queue_2->end - queue_2->begin);
                printf("Средняя длина очереди 1го типа: %lf\n", average(service, 0));
                printf("Средняя длина очереди 2го типа: %lf\n", average(service, 1));
            }
        }

        float queue_1_in = (t1_min + t1_max) * 0.5;
        float queue_1_out = (t3_min + t3_max) * 0.5; 
        float timer_modeling;

        if (queue_1_in > queue_1_out)
            timer_modeling = queue_1_in * Q1_PASSED;
        else
            timer_modeling = queue_1_out * Q1_PASSED;

        printf("\n");
        printf("\t\t-- РЕЗУЛЬТАТ --\n");
        printf("Общее время симуляции:\t\t%lf\n", service.timer);
        printf("Рассчитаное время симуляции:\t%lf\n", timer_modeling);
        printf("Погрешность:\t\t\t%lf%%\n\n", 100 * fabs(service.timer - timer_modeling) / service.timer);

        if (queue_1_in > queue_1_out)
        {
            float timer_waiting = timer_modeling - queue_1_out * Q1_PASSED; 
            float service_waiting = service.timer - service.total;

            printf("Время простоя:\t\t\t%lf\n", service_waiting);
            printf("Рассчитанное время простоя:\t%lf\n", timer_waiting);
            printf("Погрешность:\t\t\t%lf%%\n\n", 100 * fabs(service_waiting - timer_waiting) / service_waiting);
        }
        else
            printf("Время простоя:\t\t\t0\n\n");

        printf("Заявок 1го типа поступило:\t%d\n", queue_1->queue_len);
        printf("Заявок 1го типа обработано:\t%d\n", service.passed_1);
        printf("Заявок 2го типа поступило:\t%d\n", queue_2->queue_len);
        printf("Заявок 2го типа обработано:\t%d\n", service.passed_2);
    }

    return EXIT_SUCCESS;
}
