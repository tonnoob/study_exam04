#ifndef VBC_H
#define VBC_H

extern char *s;

void unexpected(char c);
int expr(void);
int factor(void);
int term(void);
int check_input(char *str);

#endif
