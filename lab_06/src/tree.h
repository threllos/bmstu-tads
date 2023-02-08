#ifndef _TREE_H_
#define _TREE_H_

typedef struct tree_node
{
    char *val;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

typedef struct
{
    tree_node **arr;
    int size;
    int mem_size;
} arr_t;

tree_node *create_node(char *val);
void fill_tree(tree_node **root, FILE *f);
double search_tree_av(tree_node *root, char **words, size_t len);
void tree_depth(tree_node *root, int *vrtxs, int *cmprs, int depth);
void store_nodes(tree_node *root, arr_t *arr);
void push_back(arr_t *arr, tree_node *to_add);
void print_tree(tree_node *root);
tree_node *build_tree(arr_t *arr, int start, int end);
tree_node *balance_tree(tree_node *root, int *unique);
void free_tree(tree_node *node);
tree_node *insert(tree_node *node, char *val);
int search_tree(char *to_find, tree_node *root);

#endif // _TREE_H_