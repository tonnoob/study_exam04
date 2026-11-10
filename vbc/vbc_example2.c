#include <stdio.h>
#include <stdlib.h> // Use the standard declarations for calloc and free.
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node    *parse_level(char **s, int min_prec); // Allows parenthesized atoms to parse a full expression.

node    *make_op(int type, node *l, node *r) // Creates an operator node and frees its children if allocation fails.
{
    node n = {type, 0, l, r};
    node *ret = new_node(n);

    if (!ret)
    {
        destroy_tree(l);
        destroy_tree(r);
    }
    return (ret);
}

node    *parse_atom(char **s) // Parses one digit or a parenthesized expression.
{
    node n;
    node *inside;

    if (isdigit((unsigned char)**s))
    {
        n = (node){VAL, *(*s)++ - '0', NULL, NULL};
        return (new_node(n));
    }
    if (accept(s, '('))
    {
        inside = parse_level(s, 1);
        if (!inside)
            return (NULL);
        if (!expect(s, ')'))
        {
            destroy_tree(inside);
            return (NULL);
        }
        return (inside);
    }
    unexpected(**s);
    return (NULL);
}

node    *parse_level(char **s, int min_prec) // Parses operators by precedence; '*' binds tighter than '+'.
{
    node *left = parse_atom(s);
    node *right;
    int type;
    int prec;

    if (!left)
        return (NULL);
    while (**s == '+' || **s == '*')
    {
        type = (**s == '+') ? ADD : MULTI;
        prec = (type == ADD) ? 1 : 2;
        if (prec < min_prec)
            break;
        (*s)++;
        right = parse_level(s, prec + 1);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        left = make_op(type, left, right);
        if (!left)
            return (NULL);
    }
    return (left);
}

node    *parse_expr(char *s) // Parses the full input and rejects trailing tokens.
{
    node *ret = parse_level(&s, 1);

    if (ret && *s)
    {
        unexpected(*s);
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return (0); // Keeps the return defined if the node type is invalid.
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]); // Builds the expression tree from the argument.
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree); // Releases the tree after evaluation.
    return (0); // Reports successful completion.
}
