#include <stdio.h>

#include "debug.h"

void debug_message(const char* message, unsigned char type) {
    if (PRODUCTION)
        printf("\033[%dm%s\033[0m\n", type, message);
}