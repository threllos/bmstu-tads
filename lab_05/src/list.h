#ifndef _LIST_H_
#define _LIST_H_

typedef struct node
{
    float data;
    struct node *next;
} node_t;

typedef struct
{
	int begin;
    int end;
    float time_wait;
    int queue_len;
    node_t **head;
} queue_list_t;

void init_list(queue_list_t *queue);
int push_list(queue_list_t *queue, float data);
float pop_list(queue_list_t *queue);
void free_list(node_t *head);

#endif // _LIST_H_
