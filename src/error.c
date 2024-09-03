#include <stdlib.h>
#include <stdio.h>

#include "error.h"

// Code 1
void jakarta_error_no_file_location(char* prefix) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_1\n");
    printf("Compiler Error: No file argument found for %s.\n", prefix);
    printf("Enter a file name or location after %s in your compiler args.\033[0m\n", prefix);
    exit(1);
}

// Code 2
void jakarta_error_file_does_not_exist(char* file_name) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_2\n");
    printf("Compiler Error: File %s does not exist.\n", file_name);
    printf("Enter a correct file name after -f.\033[0m\n");
    exit(1);
}