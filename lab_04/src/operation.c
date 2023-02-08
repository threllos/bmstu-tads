#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "operation.h"

#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;36m"
#define BLACK   "\033[0;30;47m"

int read_num_from_arr_char(char *s, size_t *ind)
{
    char num[150];
    int i = 0;

    while ((s[*ind] >= '0') && (s[*ind] <= '9'))
    {
        num[i] = s[*ind];
        i++;
        *ind += 1;
    }

    num[i] = '\0';

    int number;
    sscanf(num, "%d", &number);

    return number;
}

int translate_operation(char s)
{
    if (s == '+')
        return 1;
    else if (s == '-')
        return 2;
    else if (s == '*')
        return 3;
    else
        return 4;
}

int read_expression(exp_t *expression)
{
    printf("%sВведите выражение в следующем формате без пробелов:%s\n", GREEN, RESET);
    printf("%s{число|знак|число|знак| ... |число|знак|число}%s\n", GREEN, RESET);
    printf("%sДоступные операции: + - * /%s\n", GREEN, RESET);

    char sym;
    int rc, i = 0;
    printf("%sВведите выражение:%s ", BLUE, RESET);
    scanf("%c", &sym);

    while (((rc = scanf("%c", &sym)) == 1) && (sym != '\n'))
    {
        if ((sym > '9') || (sym < '0'))
        {
            if (!check_operation(sym))
            {
                printf("%sWar: неверный символ в выражении%s\n", YELLOW, RESET);
                return EXIT_FAILURE;
            }
        }

        if (check_operation(sym))
        {
            if ((expression->sym[i - 1] == '+') || (expression->sym[i - 1] == '-'))
            {
                printf("%sWar: не может быть введено несколько знаков подряд%s\n", YELLOW, RESET);
                return EXIT_FAILURE;    
            }
        }

        expression->sym[i++] = sym;
    }

    expression->sym[i] = '\0';
    expression->sym[i] = '\0';

    if (check_operation(expression->sym[0]))
    {
        printf("%sWar: первым должно быть введено число%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    if (check_operation(expression->sym[i - 1]))
    {
        printf("%sWar: последним должно быть введено число%s\n", YELLOW, RESET);
        return EXIT_FAILURE;
    }

    expression->len = i;

    return EXIT_SUCCESS;
}

int check_priority(int s)
{
    if ((s == 1) || (s == 2))
        return 1;

    return 2;
}

int check_operation(char s)
{
   if ((s == '+') || (s == '-') || (s == '*') || (s == '/'))
       return EXIT_FAILURE;
   else
       return EXIT_SUCCESS;
}
