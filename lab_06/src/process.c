#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tree.h"
#include "list.h"
#include "hash.h"

#define STR_LEN 100

#define RED 	"\033[0;31m"
#define GREEN 	"\033[0;32m"
#define YELLOW 	"\033[0;33m"
#define MAGENTA "\033[0;35m"
#define BLUE 	"\033[0;36m"
#define RESET 	"\033[0m"

uint64_t tick(void)
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

char **fill_words(FILE *f)
{
    rewind(f);

    size_t flen = file_len(f);

    int ind = 0;
    char buffer[STR_LEN];
    char **words = (char **)malloc(sizeof(char *) * flen);

    for (int i = 0; i < flen; ++i)
        words[i] = (char *)malloc(sizeof(char) * STR_LEN);

    while (fgets(buffer, STR_LEN, f) != NULL)
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        strcpy(words[ind], buffer);
        ind++;
    }

    rewind(f);

    return words;
}

void free_words(char **words, size_t len)
{
    for (int i = 0; i < len; ++i)
        free(words[i]);

    free(words);
}

int search_file(char *to_find, FILE *f)
{
    rewind(f);

    int i = 0;
    char buffer[STR_LEN];

    while (fgets(buffer, STR_LEN, f) != NULL)
    {
        i++;
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        if (strcmp(to_find, buffer) == 0)
            return i;
    }

    rewind(f);

    return 0;
}

double search_file_av(FILE *f, char **words, unsigned long flen)
{
    uint64_t time = tick();

    for (int i = 0; i < flen; ++i)
        search_file(words[i], f);

    time = tick() - time;

    return (double)time / flen;
}

void make_tree(FILE *f, tree_node **root)
{
	uint64_t time;

	time = tick();
    fill_tree(root, f);
    time = tick() - time;

    printf("%sБинарное дерево:%s\n", BLUE, RESET);
    print_tree(*root);
    printf("Дерево построено за %s%lld%s тактов.\n", GREEN, time, RESET);
}

void make_balance_tree(FILE *f, tree_node **root, int *count)
{
	uint64_t time;

	fill_tree(root, f);

    time = tick();
    *root = balance_tree(*root, count);
    time = tick() - time;

    printf("\n%sСбалансированное бинарное дерево:%s\n", BLUE, RESET);
    print_tree(*root);
    printf("Дерево построено за %s%lld%s тактов.\n", GREEN, time, RESET);
}

int make_unsafe_hash_table(FILE *f, list_t **hash_list, size_t size)
{
	uint64_t time;
	int max_collisions;
	*hash_list = calloc(size, sizeof(list_t));

	time = tick();
    max_collisions = build_hash_table(f, hash_list, size, &hash_rot13);
    time = tick() - time;

    printf("\n%sТаблица, созданная на основе хеш-функции Дженкинса:%s\n", BLUE, RESET);
    print_hash_table(*hash_list, size);
    printf("Максимальное количество коллизий - %s%d%s.\n", GREEN, max_collisions, RESET);
    printf("Таблица создана за %s%lld%s тактов.\n", GREEN, time, RESET);

    return max_collisions;
}

size_t make_safe_hash_table(FILE *f, list_t **hash_list, size_t size, int collisions)
{ 
	uint64_t time;
	int collisions_tmp = 0;

	while (collisions_tmp != collisions)
    {
        free_list_arr(*hash_list, size);

        size = next_prime(size);

        *hash_list = calloc(size, sizeof(list_t));

        time = tick();
        collisions_tmp = build_hash_table(f, hash_list, size, &hash_rot13);
        time = tick() - time;

        // print_hash_table(*hash_list, size);
    }

    printf("\n%sТаблица, созданная на основе хеш-функции Дженкинса:%s\n", BLUE, RESET);
    print_hash_table(*hash_list, size);
    printf("Максимальное количество коллизий - %s%d%s.\n", GREEN, collisions, RESET);
    printf("Таблица создана за %s%lld%s тактов.\n", GREEN, time, RESET);

    return size;
}

int make_search_tree(tree_node *root, char *str, char **words, size_t len, int count_words, int mode)
{
	int count = 0;
	int depth = 1;
    int vrtxs = 0;
    int cmprs = 0;
	uint64_t time;

	time = tick();
    count = search_tree(str, root);
    time = tick() - time;

    if (count > 0)
    {
    	tree_depth(root, &vrtxs, &cmprs, depth);
        printf("cmprs - %d\n vrtxs - %d\n", cmprs, vrtxs);

    	if (mode == 0)
    		printf("\n\t%sПоиск в бинарном дереве%s\n", GREEN, RESET);
    	else
    		printf("\n\t%sПоиск в бинарном сбалансированном дереве%s\n", GREEN, RESET);
    	printf("Слово найдено за:             %s%lld%s - тиков\n", GREEN, time, RESET);
    	printf("Среднее время поиска:         %s%lf%s - тиков\n", GREEN, search_tree_av(root, words, len), RESET);
    	printf("Количество сравнений:         %s%d%s\n", GREEN, count, RESET);
    	printf("Среднее количество сравнений: %s%lf%s\n", GREEN, (double) cmprs / vrtxs, RESET);
    	printf("Занимаемая память:            %s%lld%s - байт\n", GREEN, count_words * sizeof(tree_node), RESET);
    }
    else
    {
        printf("%sСлово не найдено%s\n", RED, RESET);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int make_search_hash(list_t *hash_list, char *str, size_t size, char **words, size_t len, int count_words, int collisions)
{
	int count = 0;
	uint64_t time;

	time = tick();
    count = search_hashtable(str, hash_list, size, &hash_rot13);
    time = tick() - time;

    if (count > 0)
    {
        printf("\n\t%sПоиск в хеш-таблице%s\n", GREEN, RESET);
        printf("Слово найдено за:             %s%lld%s - тиков\n", GREEN, time, RESET);
        printf("Среднее время поиска:         %s%lf%s - тиков\n", GREEN, search_hashtable_av(hash_list, size, &hash_rot13, words, len), RESET);
        printf("Количество сравнений:         %s%d%s\n", GREEN, count, RESET);
        printf("Среднее количество сравнений: %s%lf%s\n", GREEN, (double)(1 + collisions) / 2, RESET);
        printf("Занимаемая память:            %s%lld%s - байт\n", GREEN, (count_words) * sizeof(list_t) + sizeof(list_t *), RESET);
    }
    else
    {
        printf("%sСлово не найдено%s\n", RED, RESET);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int make_search_file(FILE *f, char *str, char **words, size_t len)
{
	int count;
	uint64_t time;

	time = tick();
    count = search_file(str, f);
    time = tick() - time;

    if (count > 0)
    {
        printf("\n\t%sПоиск в файле%s\n", GREEN, RESET);
        printf("Слово найдено за:             %s%lld%s - тиков\n", GREEN, time, RESET);
        printf("Среднее время поиска:         %s%lf%s - тиков\n", GREEN, search_file_av(f, words, len), RESET);
        printf("Количество сравнений:         %s%d%s\n", GREEN, count, RESET);
        printf("Среднее количество сравнений: %s%lf%s\n", GREEN, (double)file_len(f) / 2, RESET);
        printf("Занимаемая память:            %s%lld%s - байт\n", GREEN, fbytes(f), RESET);
    }
    else
    {
        printf("%sСлово не найдено%s\n", RED, RESET);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int process(FILE *f)
{
	tree_node *root = NULL, *balance_root = NULL;
	list_t *hash_list_unsafe = NULL, *hash_list_safe = NULL;
	int collisions_unsafe, collisions_safe;
	int count;
	char str[STR_LEN];
    printf("%zu\n", sizeof(int*));
	char **words = fill_words(f);
    size_t len = file_len(f);
    size_t hash_size = next_prime(len);
    size_t hash_size_2;

	make_tree(f, &root);
	make_balance_tree(f, &balance_root, &count);

	collisions_unsafe =  make_unsafe_hash_table(f, &hash_list_unsafe, hash_size);

	printf("%sВведите количество допустимых коллизий:%s ", BLUE, RESET);
	if ((scanf("%d", &collisions_safe) != 1) || (collisions_safe < 1))
	{
		printf("%sErr: некорректный ввод%s\n", RED, RESET);
		return EXIT_FAILURE;
	}

	if (collisions_unsafe > collisions_safe)
		hash_size_2 = make_safe_hash_table(f, &hash_list_safe, hash_size, collisions_safe);
	else
		printf("%sСоздавать другую таблицу не требуется.%s\n", YELLOW, RESET);

	printf("\n%sВведите слово которое хотите найти:%s ", BLUE, RESET);
    scanf("%s", str);

    if (make_search_tree(root, str, words, len, count, 0))
    	return EXIT_FAILURE;
    if (make_search_tree(balance_root, str, words, len, count, 1))
    	return EXIT_FAILURE;
    if (hash_list_safe == NULL)
    {
	    if (make_search_hash(hash_list_unsafe, str, hash_size, words, len, count, collisions_unsafe))
	    	return EXIT_FAILURE;
    }
	else
	{
		if (make_search_hash(hash_list_safe, str, hash_size_2, words, len, count, collisions_safe))
	    	return EXIT_FAILURE;
	}
    make_search_file(f, str, words, len);

    free_tree(root);
    free_tree(balance_root);
    free_words(words, len);
    free_list_arr(hash_list_unsafe, hash_size);
    if (hash_list_safe != NULL)
    {
    	free_list_arr(hash_list_safe, hash_size_2);
    }

	return EXIT_SUCCESS;
}