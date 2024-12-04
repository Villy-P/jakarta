#include <stddef.h>

#include "postfix.h"

unsigned int precedence(char* op) {
    if (op == NULL)
        return 0;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
}