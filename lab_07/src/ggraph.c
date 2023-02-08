#include <stdio.h>
#include <stdlib.h>
#include "ggraph.h"

#define RED 	"\033[0;31m"
#define GREEN 	"\033[0;32m"
#define YELLOW 	"\033[0;33m"
#define MAGENTA "\033[0;35m"
#define BLUE 	"\033[0;36m"
#define RESET 	"\033[0m"

#define INF 1000000

void free_graph(int **graph, int node)
{
	for (int i = 0; i < node; i++)
		free(graph[i]);

	free(graph);
}

int added_new_route(int **graph, int size, int route_len, int node, int max_len)
{
	int freed_route = 0;
	for (int i = 0; i < size - 1; i++)
		for (int j = 1 + i; j < size; j++)
			if (graph[i][j] == 0)
				freed_route += 1;

	if (freed_route > 0)
	{
		for (int i = 0; i < size - 1; i++)
			for (int j = 1 + i; j < size; j++)
				if (graph[i][j] == 0)
				{
					graph[i][j] = route_len;
					graph[j][i] = route_len;

					if(!find_len(graph, size, node, max_len))
					{
						printf("%sДобавлена одна дорога%s\n", GREEN, RESET);
						int *index = malloc(sizeof(int) * 2);
						index[0] = i;
						index[1] = j;
						print_graph(graph, size, 1, index);
						free(index);
						return EXIT_SUCCESS;
					}
					
					graph[i][j] = 0;
					graph[j][i] = 0;
				}
	}

	if (freed_route > 1)
	{
		for (int i = 0; i < size - 1; i++)
			for (int j = 1 + i; j < size; j++)
				if (graph[i][j] == 0)
				{
					graph[i][j] = route_len;
					graph[j][i] = route_len;

					for (int n = 0; n < size - 1; n++)
						for (int m = 1 + n; m < size; m++)
							if (graph[n][m] == 0)
							{
								graph[n][m] = route_len;
								graph[m][n] = route_len;

								if(!find_len(graph, size, node, max_len))
								{
									printf("%sДобавлены две дороги%s\n", GREEN, RESET);
									int *index = malloc(sizeof(int) * 4);
									index[0] = i;
									index[1] = j;
									index[2] = n;
									index[3] = m;
									print_graph(graph, size, 2, index);
									free(index);
									return EXIT_SUCCESS;
								}

								graph[n][m] = 0;
								graph[m][n] = 0;
							}

					graph[i][j] = 0;
					graph[j][i] = 0;
				}
	}

	if (freed_route > 2)
	{
		for (int i = 0; i < size - 1; i++)
			for (int j = 1 + i; j < size; j++)
				if (graph[i][j] == 0)
				{
					graph[i][j] = route_len;
					graph[j][i] = route_len;

					for (int n = 0; n < size - 1; n++)
						for (int m = 1 + n; m < size; m++)
							if (graph[n][m] == 0)
							{
								graph[n][m] = route_len;
								graph[m][n] = route_len;

								for (int k = 0; k < size - 1; k++)
									for (int l = 1 + k; l < size; l++)
										if (graph[k][l] == 0)
										{
											graph[k][l] = route_len;
											graph[l][k] = route_len;

											if(!find_len(graph, size, node, max_len))
											{
												printf("%sДобавлены три дороги%s\n", GREEN, RESET);
												int *index = malloc(sizeof(int) * 6);
												index[0] = i;
												index[1] = j;
												index[2] = n;
												index[3] = m;
												index[4] = k;
												index[5] = l;
												print_graph(graph, size, 3, index);
												free(index);
												return EXIT_SUCCESS;
											}

											graph[k][l] = 0;
											graph[l][k] = 0;
										}

								graph[n][m] = 0;
								graph[m][n] = 0;
							}

					graph[i][j] = 0;
					graph[j][i] = 0;
				}
	}

	return EXIT_FAILURE;
}

int find_len(int **graph, int size, int num, int len)
{
	int *lens = malloc(size * sizeof(int));
	int *f = malloc (size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		if (i != num - 1)
			lens[i] = INF;
		else
			lens[i] = 0;
		f[i] = 0;
	}

	int minindex;
	int min;

	do
	{
		minindex = INF;
		min = INF;

		for (int i = 0; i < size; i++)
		{
			if (f[i] == 0 && lens[i]<min)
			{
				min = lens[i];
				minindex = i;
			}
		}

		if (minindex != INF)
		{
			for (int i = 0; i < size; i++)
			{
				if (graph[minindex][i] > 0)
				{
					int tmp = min + graph[minindex][i];
					if (tmp < lens[i])
						lens[i] = tmp;
				}

			}

			f[minindex] = 1;
		}
	} while (minindex < INF);

	for (int i = 0; i < size; i++)
		if (lens[i] > len)
		{
			free(lens);
			free(f);
			return EXIT_FAILURE;
		}

	free(lens);
	free(f);
	return EXIT_SUCCESS;
}

int input_graph(int **graph, int node, int route)
{
	for (int i = 0; i < route; i++)
	{
		int n1, n2, w;

		printf("%sВведите (вершина1, вершина2, длина дорого): %s ", BLUE, RESET);
		if (scanf("%d%d%d", &n1, &n2, &w) != 3)
		{
			printf("%sНекорректный ввод%s\n", RED, RESET);
			free_graph(graph, node);
			return EXIT_FAILURE;
		}

		if (n1 < 1 || n1 > node)
		{
			printf("%sНекорректный ввод%s\n", RED, RESET);
			free_graph(graph, node);
			return EXIT_FAILURE;
		}

		if (n2 < 1 || n2 > node)
		{
			printf("%sНекорректный ввод%s\n", RED, RESET);
			free_graph(graph, node);
			return EXIT_FAILURE;
		}

		if (w < 0)
		{
			printf("%sНекорректный ввод%s\n", RED, RESET);
			free_graph(graph, node);
			return EXIT_FAILURE;
		}

		if (!graph[n1 - 1][n2 - 1])
		{
			graph[n1 - 1][n2 - 1] = w;
			graph[n2 - 1][n1 - 1] = w;
		}
		else
		{
			printf("%sПуть уже записан%s\n", RED, RESET);
			return EXIT_FAILURE;
		}
	}

	printf("%sГраф считан%s\n", GREEN, RESET);

	return EXIT_SUCCESS;
}

void print_graph(int **graph, int size, int fl, int *a)
{
	FILE *f;

	f = fopen("1.gv", "w");

	fprintf(f, "graph test {\n");

	for (int i = 0; i < size - 1; i++)
		for (int j = 1 + i; j < size; j++)
			if (graph[i][j] > 0)
			{
				if ((fl == 1 || fl == 2 || fl == 3) && ((a[0] == i && a[1] == j) || (a[0] == j && a[1] == i)))
					fprintf(f, "  %d -- %d [label=%d, color=red];\n", i + 1, j + 1, graph[i][j]);
				else if ((fl == 2 || fl == 3) && ((a[2] == i && a[3] == j) || (a[2] == j && a[3] == i)))
					fprintf(f, "  %d -- %d [label=%d, color=red];\n", i + 1, j + 1, graph[i][j]);
				else if (fl == 3 && ((a[4] == i && a[5] == j) || (a[4] == j && a[5] == i)))
					fprintf(f, "  %d -- %d [label=%d, color=red];\n", i + 1, j + 1, graph[i][j]);
				else
					fprintf(f, "  %d -- %d [label=%d];\n", i + 1, j + 1, graph[i][j]);
			}

	fprintf(f, "}");

	fclose(f);
	system("dot -Tpng 1.gv -o 1.png");
	system("start 1.png");
}