#ifndef _HASH_H_
#define _HASH_H_

#include <stdio.h>
#include "list.h"

size_t next_prime(int n);
size_t fbytes(FILE *f);
int search_hashtable(char *to_find, list_t *hash_list, int n, size_t (*hash)(char *, int));
int build_hash_table(FILE *f, list_t **arr, size_t size, size_t (*hash)(char *, int));
void print_hash_table(list_t *arr, size_t size);
size_t hash_rot13(char *str, int n);
size_t file_len(FILE *f);
double search_hashtable_av(list_t *hash_list, int n, size_t (*hash)(char *, int), char **words, size_t len);
int is_prime(size_t num);

#endif // _HASH_H_