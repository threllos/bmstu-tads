#include <stdio.h>
#include <stdlib.h>
#include "ggraph.h"

#define RED 	"\033[0;31m"
#define GREEN 	"\033[0;32m"
#define YELLOW 	"\033[0;33m"
#define MAGENTA "\033[0;35m"
#define BLUE 	"\033[0;36m"
#define RESET 	"\033[0m"

int input_sizes(int *node, int *route)
{
	printf("%sВведите количество вершин:%s ", BLUE, RESET);
	if (scanf("%d", node) != 1)
	{
		printf("%sНекорректный ввод%s\n", RED, RESET);
		return EXIT_FAILURE;
	}

	printf("%sВведите количество ребер:%s ", BLUE, RESET);
	if (scanf("%d", route) != 1)
	{
		printf("%sНекорректный ввод%s\n", RED, RESET);
		return EXIT_FAILURE;
	}

	if (*node < 0 || *route < 0)
	{
		printf("%sВведено отрицательное число%s\n", RED, RESET);
		return EXIT_FAILURE;
	}

	if ((*node) * (*node - 1) / 2 < *route)
	{
		printf("%sОчень много ребер%s\n", RED, RESET);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int main(void)
{
	setbuf(stdout, NULL );

	int **graph = NULL;
	int count_node;
	int count_route;
	int node;
	int max_len;

	if (input_sizes(&count_node, &count_route))
		return EXIT_FAILURE;

	graph = calloc(count_node, sizeof(int *));
	if (!graph)
	{
		printf("%sНе выделилась память%s\n", RED, RESET);
		return EXIT_FAILURE;
	}
	for (int i = 0; i < count_node; i++)
	{
		graph[i] = malloc(count_node * sizeof(int));

		if (!graph[i])
		{
			printf("%sНе выделилась память%s\n", RED, RESET);
			free_graph(graph, count_node);
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < count_node; i++)
		for (int j = 0; j < count_node; j++)
			graph[i][j] = 0;

	if (input_graph(graph, count_node, count_route))
		return EXIT_FAILURE;

	print_graph(graph, count_node, 0, NULL);

	printf("%sВведите вершину для поиска:%s ", BLUE, RESET);
	if (scanf("%d", &node) != 1)
	{
		printf("%sНекорректный ввод%s\n", RED, RESET);
		free_graph(graph, count_node);
		return EXIT_FAILURE;
	}
	if (node < 1 || node > count_node)
	{
		printf("%sНекорректная вершина%s\n", RED, RESET);
		free_graph(graph, count_node);
		return EXIT_FAILURE;
	}

	printf("%sВведите максимальный путь%s: ", BLUE, RESET);
	if (scanf("%d", &max_len) != 1)
	{
		printf("%sНекорректный ввод%s\n", RED, RESET);
		free_graph(graph, count_node);
		return EXIT_FAILURE;
	}
	if (max_len < 1)
	{
		printf("%sНекорректная длина пути%s\n", RED, RESET);
		free_graph(graph, count_node);
		return EXIT_FAILURE;
	}

	if(!find_len(graph, count_node, node, max_len))
		printf("%sДобавлять дороги не требуется%s\n", GREEN, RESET);
	else
	{
		int len_route;

		printf("%sВведите длину дорог, которые будут добавляться:%s ", BLUE, RESET);
		if (scanf("%d", &len_route) != 1)
		{
			printf("%sНекорректный ввод%s\n", RED, RESET);
			free_graph(graph, count_node);
			return EXIT_FAILURE;
		}

		if (len_route < 1)
		{
			printf("%sНекорректая длина%s\n", RED, RESET);
			free_graph(graph, count_node);
			return EXIT_FAILURE;
		}

		if (added_new_route(graph, count_node, len_route, node, max_len))
			printf("%sДобраться до всех городов за %d ед. невозможно%s\n", YELLOW, max_len, RESET);
		else
			printf("%sДобраться до всех городов за %d ед. возможно%s\n", GREEN, max_len, RESET);
	}

	free_graph(graph, count_node);

	return EXIT_SUCCESS;
}