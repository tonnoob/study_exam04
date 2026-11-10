#include <stdio.h>
#include <stdlib.h>
#include "vbc.h"

char *s;

void unexpected(char c)
{
    if (c == '\0')
        printf("Unexpected end of file\n");
    else
        printf("Unexpected token '%c'\n", c);
}

int term(void)
{
    int n = 0;
    if (*s >= '0' && *s <= '9')
        return (*s++ - '0');
    while (*s == '(')
    {
        s++;
        n = expr();
        s++;
    }
    return n;
}

int factor(void)
{
    int n = term();
    while (*s == '*')
    {
        s++;
        n *= term();
    }
    return n;
}

int expr(void)
{
    int n = factor();
    while (*s == '+')
    {
        s++;
        n += factor();
    }
    return n;
}

int check_input(char *str)
{
    int parentheses = 0;
    int i = 0;
    char last = '\0';

    while (str[i])
    {
        if (str[i] == '(')
            parentheses++;
        else if (str[i] == ')')
            parentheses--;
        else if ((str[i] < '0' || str[i] > '9') &&
                 str[i] != '+' && str[i] != '*')
        {
            unexpected(str[i]);
            return 1;
        }
        if (str[i] >= '0' && str[i] <= '9' &&
            str[i + 1] >= '0' && str[i + 1] <= '9')
        {
            unexpected(str[i + 1]);
            return 1;
        }
        last = str[i];
        i++;
    }
    if (parentheses > 0)
    {
        unexpected('(');
        return 1;
    }
    if (parentheses < 0)
    {
        unexpected(')');
        return 1;
    }
    if (last == '+' || last == '*')
    {
        unexpected('\0');
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int result;

    if (argc != 2)
        return 1;
    if (check_input(argv[1]))
        return 1;
    s = argv[1];
    result = expr();
    printf("%d\n", result);
    return 0;
}