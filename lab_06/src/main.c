#include <stdio.h>
#include <stdlib.h>
#include "process.h"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define MAGENTA "\033[0;35m"
#define BLUE "\033[0;36m"
#define RESET "\033[0m"

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    FILE *f = fopen(argv[1], "r");

    if (!f)
    {
        printf("%sErr: неверное имя файла%s\n", RED, RESET);
        return EXIT_FAILURE;
    }

    if (fgetc(f) == EOF)
    {
        printf("%sErr: пустой файл%s\n", RED, RESET);
        return EXIT_FAILURE;
    }

    rewind(f);

    if (process(f))
    {
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);

    return EXIT_SUCCESS;
}