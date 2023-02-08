#include <stdio.h>
#include <stdlib.h>
#include "list.h"

node_t *init_node(float data)
{
    node_t *node = malloc(sizeof(node_t));

    if (!node)
        return NULL;

    node->next = NULL;
    node->data = data;

    return node;
}

float pop_list(queue_list_t *queue)
{
    if ((*queue->head) == NULL)
        return EXIT_FAILURE;
    
    float tmp = (*queue->head)->data;

    node_t *node;
    node = (*queue->head)->next;

    free(*queue->head);

    *queue->head = node;
    queue->end--;

    return tmp;
}

void free_list(node_t *head)
{
    while (head)
    {
        node_t *tmp = head->next;

        free(head);

        head = tmp;
    }
}   

int push_list(queue_list_t *queue, float data)
{
    node_t *node = init_node(data);

    node_t *tmp_node = *queue->head;
    if (tmp_node)
    {
        while (tmp_node->next != NULL)
            tmp_node = tmp_node->next;

        tmp_node->next = node;
    }
    else
        *queue->head = node;

    queue->end++;
    return EXIT_SUCCESS;
}

void init_list(queue_list_t *queue)
{
    queue->head = malloc(sizeof(node_t *));
    queue->queue_len = 0;
    queue->time_wait = 0.0;
    queue->begin = 0;
    queue->end = 0;
    *queue->head = NULL;
}