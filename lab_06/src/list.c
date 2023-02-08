#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void free_list(list_t *node)
{
    if (!node)
        return;

    free_list(node->next);

    free(node);
}

void free_list_arr(list_t *arr, int size)
{
    if (!arr)
        return;
    
    for (size_t i = 0; i < size; i++)
        free_list(arr[i].next);

    free(arr);
}

int deepcmp(char *s, list_t l)
{
    list_t *lp = &l;
    int i = 0;

    while (lp != NULL && lp->value != NULL)
    {
        i++;
        if (strcmp(s, lp->value) == 0)
            return i;
        lp = lp->next;
    }

    return 0;
}

int list_occupation(list_t *arr, int size)
{
    int occupation = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (arr[i].value)
            occupation++;
    }

    return occupation;
}

int linsert(list_t *arr, char *buffer)
{
    if (!arr->next)
    {
        arr->next = calloc(1, sizeof(list_t));
        arr->value = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(arr->value, buffer);

        return 1;
    }

    int depth = 1;
    depth += linsert(arr->next, buffer);

    return depth;
}
