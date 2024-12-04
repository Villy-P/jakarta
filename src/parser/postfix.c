#include <stddef.h>

#include "postfix.h"

unsigned int precedence(char* operator) {
    if (operator == NULL)
        return 0;
    if (strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0)
        return 1;
    if (strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0)
        return 2;
}