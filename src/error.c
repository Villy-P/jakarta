#include <stdlib.h>

#include "error.h"

void jakarta_error(int error_code) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_%d\n", error_code);
    switch (error_code) {
        case 1:
            printf("Compiler Error: No file argument found for -f.\n");
            printf("Enter a file name or location after -f in your compiler args.\033[0m\n");
            break;
    }
    exit(1);
}