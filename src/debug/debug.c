#include <stdint.h>
#include <stdio.h>

#include "debug.h"

void debug_message(const char* message, uint8_t type) {
    if (PRODUCTION) {
        printf("\033[%dm%s\033[0m\n", type, message);
    }
}