#include <stdio.h>
#include <stdlib.h>
#include "array.h"

int push_arr(queue_arr_t *queue, float time)
{
    if (queue->end < MAX_SIZE - 1)
        queue->queue[queue->end++] = time;
    else
    {
        printf("overflow\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void init_arr(queue_arr_t *queue)
{
    queue->time_wait = 0.0;
    queue->end = 0;
    queue->queue_len = 0;
    queue->begin = 0;
}

float pop_arr(queue_arr_t *queue)
{
    float temp = queue->queue[0];

    if (queue->begin == queue->end)
    {
        printf("underflow\n");
        return EXIT_FAILURE;
    }

    for (size_t i = queue->begin; i < queue->end; i++)
        queue->queue[i] = queue->queue[i + 1];

    queue->end--;
    return temp;
}
