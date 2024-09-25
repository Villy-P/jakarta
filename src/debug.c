#include <stdio.h>

#include "debug.h"

void debug_message(char* message) {
    if (PRODUCTION)
        printf("%s", message);
}