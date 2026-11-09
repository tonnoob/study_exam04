#include <stdio.h>
#include <stdlib.h>

char *s;

void unexpected(char c);
int expr(void);
int factor(void);
int term(void);

void unexpected(char c)
{
    if (c == '\0')
        printf("Unexpected end of input\n");
    else
        printf("Unexpected token '%c'\n", c);
    exit(1);
}

int term(void)
{
    int n;

    // parentese
    if (*s == '(')
    {
        s++;
        n = expr();
        if (*s != ')')
            unexpected(*s);
        s++;
        return n;
    }

    if (*s >= '0' && *s <= '9')
    {
        n = *s - '0';
        s++;
        return n;
    }

    unexpected(*s);
    return 0;
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

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    s = argv[1];

    if (*s == '\0')
        unexpected(*s);

    printf("%d\n", expr());

    return 0;
}