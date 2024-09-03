#include <stdlib.h>

#include "error.h"

// Code 1
void jakarta_error_no_file_location() {
    printf("\033[31mThere was an error while running your code: ERR_CODE_1\n");
    printf("Compiler Error: No file argument found for -f.\n");
    printf("Enter a file name or location after -f in your compiler args.\033[0m\n");
    exit(1);
}