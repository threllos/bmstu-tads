#ifndef _ARRAY_H_
#define _ARRAY_H_

#define MAX_SIZE 3000

typedef struct
{
    int begin;
    int end;
    float time_wait;
    int queue_len;
    float queue[MAX_SIZE];
} queue_arr_t;

void init_arr(queue_arr_t *queue);
float pop_arr(queue_arr_t *queue);
int push_arr(queue_arr_t *queue, float time);

#endif // _ARRAY_H_
