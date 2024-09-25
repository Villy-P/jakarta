#include <stdio.h>

#include "debug.h"

void debug_message(char* message) {
    if (PRODUCTION)
        printf("\033[32m%s\033[0m\n", message);
}