#ifndef _LIST_H_
#define _LIST_H_

typedef struct list
{
    char *value;
    struct list *next;
} list_t;

void free_list_arr(list_t *arr, int size);
int linsert(list_t *arr, char *buffer);
int list_occupation(list_t *arr, int size);
int deepcmp(char *s, list_t l);

#endif // _LIST_H_