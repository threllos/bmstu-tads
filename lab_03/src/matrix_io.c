#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "matrix_io.h"
#include "matrix_struct.h"

#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;36m"
#define BLACK   "\033[0;30;47m"

void print_info()
{
    printf("\nУмножение вектор-строки на матрицу, используя:\n");
    printf("\t- Алгоритм для обработки разряженных матриц;\n");
    printf("\t- Алгоритм стандартной обработки матрицы.\n");
    printf("И вывод сравнения алгоритмов.\n\n");
    printf("=========================++=========================\n\n");
    printf("0. Сгенерировать случайную матрицу и вектор-строку.\n");
    printf("1. Заполнить матрицу и вектор-строку самостоятельно.\n");
    printf("%sВведите команду:%s ", BLUE, RESET);
}

void print_enter_data(matrix_t matrix, matrix_t vector)
{
    if (matrix.columns * matrix.rows > 400)
    {
        printf("\n%sМатрица имеет слишком большой размер.%s\n", YELLOW, RESET);
        return;
    }

    printf("\n%sВектор:%s\n", GREEN, RESET);
    printf("%s", BLACK);
    for (size_t i = 0; i < vector.columns - 1; i++)
        printf("%3d ", vector.matrix[0][i]);
    printf("%3d%s\n", vector.matrix[0][vector.columns - 1], RESET);

    printf("\n%sМатрица:%s\n", GREEN, RESET);
    for (size_t i = 0; i < matrix.rows; i++)
    {
        printf("%s", BLACK);
        for (size_t j = 0; j < matrix.columns - 1; j++)
            printf("%3d ", matrix.matrix[i][j]);
        printf("%3d%s\n", matrix.matrix[i][matrix.columns - 1], RESET);
    }
}

void print_result(matrix_t std_matrix)
{
    if (std_matrix.columns > 30)
    {
        printf("\n%sРезультат имеет слишком большой размер.%s\n", YELLOW, RESET);
        return;
    }

    printf("\n%sРезультат вектор-строка:%s\n", GREEN, RESET);
    printf("%s", BLACK);
    for (size_t i = 0; i < std_matrix.columns - 1; i++)
        printf("%d ", std_matrix.matrix[0][i]);
    printf("%d%s\n",std_matrix.matrix[0][std_matrix.columns - 1], RESET);
}

void print_sparse(sparse_t matrix)
{
    if (matrix.curr_size == 0)
    {
        printf("\n%sРазряженный вектор пуст.%s\n", YELLOW, RESET);
        return;
    }

    if (matrix.curr_size > 30)
    {
        printf("\n%sРазряженный вектор имеет слишком большой размер.%s\n", YELLOW, RESET);
        return;
    }

    printf("\n%sРазряженный вектор:%s\n", GREEN, RESET);
    printf("%s", BLACK);
    for (size_t i = 0; i < matrix.curr_size - 1; i++)
        printf("%3d ", matrix.A[i]);
    printf("%3d%s\n", matrix.A[matrix.curr_size - 1], RESET);

    printf("\n%sИндексы разряженного вектора:%s\n", GREEN, RESET);
    printf("%s", BLACK);
    for (size_t i = 0; i < matrix.curr_size - 1; i++)
        printf("%3d ", matrix.JA[i]);
    printf("%3d%s\n", matrix.JA[matrix.curr_size - 1], RESET);
}

void print_data(matrix_t std_matrix, sparse_t sparse_matrix,
    int64_t std_start, int64_t std_end, int64_t sparse_end, int curr_size)
{
    double one_percent = (double)(std_matrix.rows) * std_matrix.columns / 100;
    double how_many_in = sparse_matrix.curr_size  / one_percent;

    printf("\nРезультаты сравнения:\n");
    printf("Для матрицы размера %zux%zu и заполненности %.1lf%%\n", std_matrix.rows, std_matrix.columns, how_many_in);

    printf("\n%sВремя:%s\n", GREEN, RESET);
    printf("%s+---------------------+--------------------+%s\n", BLACK, RESET);
    printf("%s| Обычная матрица     | %18lld |%s\n", BLACK, std_end - std_start, RESET);
    printf("%s| Разряженная матрица | %18lld |%s\n", BLACK, sparse_end - std_end, RESET);
    printf("%s+---------------------+--------------------+%s\n", BLACK, RESET);

    printf("\n%sПамять:%s\n", GREEN, RESET);
    printf("%s+---------------------+--------------------+%s\n", BLACK, RESET);
    printf("%s| Обычная матрица     | %18lld |%s\n", BLACK, sizeof(int) * std_matrix.rows * std_matrix.columns, RESET);
    printf("%s| Разряженная матрица | %18lld |%s\n", BLACK, 2 * sizeof(int) * curr_size + sizeof(node_t) * std_matrix.columns, RESET);
    printf("%s+---------------------+--------------------+%s\n", BLACK, RESET);
}

int read_sizes(sparse_t *matrix, sparse_t *vector)
{
    long curr_size;

    printf("%sВведите количество строк матрицы:%s ", BLUE, RESET);
    if (scanf("%zu", &matrix->rows) != 1)
        return EXIT_FAILURE;

    if (matrix->rows < 2 || matrix->rows > 10000)
    {
        printf("%sНекорректное количество строк.%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    printf("%sВведите количество столбцов матрицы:%s ", BLUE, RESET);
    if (scanf("%zu", &matrix->columns) != 1)
        return EXIT_FAILURE;

    if (matrix->columns < 2 || matrix->columns > 10000)
    {
        printf("%sНекорректное количество столбцов.%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    printf("%sВведите количество ненулевых элементов матрицы:%s ", BLUE, RESET);
    if (scanf("%ld", &curr_size) != 1)
        return EXIT_FAILURE;

    if (curr_size > 2147483646)
    {
        printf("%sКоличество элементов привышает максимум.%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    matrix->curr_size = curr_size;
    long long size = (long long)matrix->columns * matrix->rows;

    if (matrix->curr_size < 1 || matrix->curr_size > size)
    {
        printf("%sНекорректное количество ненулевых элементов.%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    printf("%sВведите количество ненулевых элементов вектор-строки:%s ", BLUE, RESET);
    if (scanf("%d", &vector->curr_size) != 1)
        return EXIT_FAILURE;

    if (vector->curr_size < 1 || vector->curr_size > matrix->rows)
    {
        printf("%sНекорректное количество ненулевых элементов.%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int read_type(int *enter_type)
{
    int type;

    if (scanf("%d", &type) != 1)
        return EXIT_FAILURE;

    if (type < 0 || type > 1)
    {
        printf("%sНекорректный выбор меню.%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    *enter_type = type;

    return EXIT_SUCCESS;
}

int read_matrix(matrix_t *std_matrix, int maxsize)
{
    for (int i = 0; i < std_matrix->rows; i++)
        for (int j = 0; j < std_matrix->columns; j++)
            std_matrix->matrix[i][j] = 0;

    int numb, i1, j1;

    for (int i = 0; i < maxsize; i++)
    {
        printf("%sВведите (элемент матрицы, строку, столбец):%s ", BLUE, RESET);
        if (scanf("%d %d %d", &numb, &i1, &j1) != 3)
            return EXIT_FAILURE;

        if (i1 < 0 || i1 >= std_matrix->rows)
        {
            printf("%sНекорректный ввод строки.%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }

        if (j1 < 0 || j1 >= std_matrix->columns)
        {
            printf("%sНекорректный ввод столбца.%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }

        if (numb == 0)
        {
            printf("%sНекорректный ввод элемента матрицы.%s\n", YELLOW, RESET);
            return EXIT_FAILURE; 
        }

        std_matrix->matrix[i1][j1] = numb;
    }

    return EXIT_SUCCESS;
}

int read_vector(matrix_t *std_matrix, int maxsize)
{
    int numb, i1;

    for (int i = 0; i < maxsize; i++)
    {
        printf("%sВведите (элемент вектора-строки,  столбец):%s ", BLUE, RESET);
        if (scanf("%d %d", &numb, &i1) != 2)
            return EXIT_FAILURE;

        if (i1 < 0 || i1 >= std_matrix->columns)
        {
            printf("%sНекорректный ввод столбца.%s\n", YELLOW, RESET);
            return EXIT_FAILURE;
        }

        if (numb == 0)
        {
            printf("%sНекорректный ввод элемента вектора.%s\n", YELLOW, RESET);
            return EXIT_FAILURE; 
        }


        std_matrix->matrix[0][i1] = numb;
    }

    return EXIT_SUCCESS;
}
