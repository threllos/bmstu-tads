#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "hash.h"

#define STR_LEN 100

uint64_t tick_hash(void)
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

int search_hashtable(char *to_find, list_t *hash_list, int n, size_t (*hash)(char *, int))
{
    int ec = deepcmp(to_find, hash_list[hash(to_find, n)]);

    if (ec > 0)
        return ec;

    return 0;
}

size_t fbytes(FILE *f)
{
    rewind(f);
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    return size;
}

double search_hashtable_av(list_t *hash_list, int n, size_t (*hash)(char *, int),
                           char **words, size_t len)
{
    uint64_t time = tick_hash();

    for (size_t i = 0; i < len; ++i)
        search_hashtable(words[i], hash_list, n, hash);

    time = tick_hash() - time;

    return (double)time / len;
}

size_t hash_rot13(char *str, int n)
{
    size_t hash = 0;

    while (*str)
    {
        hash += *str++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % n;
}

size_t file_len(FILE *f)
{
    size_t lines = 0;
    char buffer[STR_LEN];

    rewind(f);

    while (fgets(buffer, STR_LEN, f) != NULL)
        lines++;

    rewind(f);

    return lines;
}

size_t next_prime(int n)
{
    size_t prime = n + 1;

    while (!is_prime(prime))
        prime++;

    return prime;
}

int build_hash_table(FILE *f, list_t **arr, size_t size, size_t (*hash)(char *, int))
{
    char buffer[STR_LEN];
    int max_collisions = 0;

    while (fgets(buffer, STR_LEN, f))
    {
        int cur_collisions = 0;

        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        if (deepcmp(buffer, (*arr)[hash(buffer, size)]) == 0)
            cur_collisions = linsert(&(*arr)[hash(buffer, size)], buffer);

        if (cur_collisions > max_collisions)
            max_collisions = cur_collisions;
    }

    rewind(f);

    return max_collisions;
}

void print_hash_table(list_t *arr, size_t size)
{
    list_t *head;

    printf("+-----+----------------------------------------+\n");
    printf("| %3s | %s\n", "ХЭШ", "ДАННЫЕ");
    printf("+-----+----------------------------------------+\n");

    for (size_t i = 0; i < size; i++)
    {
        int flag_first_word = 1;
        
        head = &arr[i];

        printf("| %3zu | ", i);

        while (head)
        {
            if (head->value)
            {
                if (flag_first_word)
                {
                    printf("%s", head->value);
                    flag_first_word = 0;
                }
                else
                    printf(", %s", head->value);
            }

            head = head->next;
        }

        printf("\n+-----+----------------------------------------+\n");
    }
}

int is_prime(size_t num)
{
    if (num <= 1)
        return 0;

    if (num % 2 == 0 && num > 2)
        return 0;

    for (size_t i = 3; i < floor(sqrt(num)); i += 2)
        if (num % i == 0)
            return 0;

    return 1;
}
