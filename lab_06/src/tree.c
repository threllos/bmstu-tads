#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tree.h"

#define STR_LEN 100
#define SPACE 5

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define MAGENTA "\033[0;35m"
#define RESET "\033[0m"

void print_tree_next(tree_node *root, int space, int direction);

uint64_t tick_tree(void)
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

tree_node *build_tree(arr_t *arr, int start, int end)
{
    if (start > end)
        return NULL;

    int mid = (start + end) / 2;
    tree_node *root = arr->arr[mid];

    root->right = build_tree(arr, start, mid - 1);
    root->left = build_tree(arr, mid + 1, end);

    return root;
}

tree_node *create_node(char *val)
{
    tree_node *node = malloc(sizeof(tree_node));

    node->val = malloc(sizeof(char *) * (strlen(val) + 1));
    strcpy(node->val, val);

    node->left = NULL;
    node->right = NULL;

    return node;
}

void free_tree(tree_node *node)
{
    if (!node)
        return;

    free_tree(node->left);
    free_tree(node->right);

    free(node);
}

tree_node *insert(tree_node *node, char *val)
{
    if (!node)
        return create_node(val);

    if (strcmp(val, node->val) > 0) 
        node->left = insert(node->left, val);
    else if (strcmp(val, node->val) < 0)
        node->right = insert(node->right, val);

    return node;
}

void fill_tree(tree_node **root, FILE *f)
{
    char buffer[STR_LEN];

    fgets(buffer, STR_LEN, f);
    if (buffer[strlen(buffer) - 1] == '\n')
        buffer[strlen(buffer) - 1] = '\0';

    *root = insert(*root, buffer);

    while (fgets(buffer, STR_LEN, f))
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        insert(*root, buffer);
    }

    rewind(f);
}

void print_tree(tree_node *root)
{
    printf(GREEN);
    print_tree_next(root, 0, 0);
    printf(RESET);
}

void print_tree_next(tree_node *root, int space, int direction)
{
    if (!root)
        return;

    space += SPACE;

    print_tree_next(root->right, space, 1);

    for (size_t i = SPACE; i < space - 2; i++)
        printf(" ");

    if (direction == 1)
        printf("┌ %s\n", root->val);
    else if (direction == 2)
        printf("└ %s\n", root->val);
    else
        printf("%s\n", root->val);

    print_tree_next(root->left, space, 2);
}

void tree_depth(tree_node *root, int *vrtxs, int *cmprs, int depth)
{
    if (!root)
        return;

    (*vrtxs)++;
    (*cmprs) += depth;

    tree_depth(root->right, vrtxs, cmprs, depth + 1);
    tree_depth(root->left, vrtxs, cmprs, depth + 1);
}

void push_back(arr_t *arr, tree_node *to_add)
{
    arr->size++;

    if (arr->size >= arr->mem_size)
    {
        arr->mem_size *= 2;
        arr->arr = realloc(arr->arr, arr->mem_size * sizeof(tree_node *));
    }

    for (size_t i = arr->size; i > 0; i--)
        arr->arr[i] = arr->arr[i - 1];

    arr->arr[0] = to_add;
}

int search_tree(char *to_find, tree_node *root)
{
    int i = 0;
    tree_node *p = root;

    while (p)
    {
        i++;

        if (strcmp(p->val, to_find) == 0)
            return i;

        if (strcmp(p->val, to_find) > 0)
            p = p->right;
        else
            p = p->left;
    }

    return 0;
}

double search_tree_av(tree_node *root, char **words, size_t len)
{
    uint64_t time = tick_tree();

    for (int i = 0; i < len; ++i)
        search_tree(words[i], root);

    time = tick_tree() - time;

    return (double)time / len;
}

tree_node *balance_tree(tree_node *root, int *unique)
{
    arr_t arr;
    arr.mem_size = 8;
    arr.size = 0;

    arr.arr = malloc(sizeof(tree_node *) * arr.mem_size);
    store_nodes(root, &arr);

    int n = arr.size;
    *unique = n;

    tree_node *r = build_tree(&arr, 0, n - 1);

    free(arr.arr);

    return r;
}

void store_nodes(tree_node *root, arr_t *arr)
{
    if (!root)
        return;

    store_nodes(root->left, arr);
    push_back(arr, root);
    store_nodes(root->right, arr);
}
