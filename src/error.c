#include <stdlib.h>
#include <stdio.h>

#include "error.h"

#define DEFAULT_ERROR_CODE 1

// Code 1
void jakarta_error_no_file_location(const char* prefix) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_1\n");
    printf("Compiler Error: No file argument found for %s.\n", prefix);
    printf("Enter a file name or location after %s in your compiler args.\033[0m\n", prefix);
    exit(DEFAULT_ERROR_CODE);
}

// Code 2
void jakarta_error_file_does_not_exist(const char* file_name) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_2\n");
    printf("Compiler Error: File %s does not exist.\n", file_name);
    printf("Enter a correct file name after -f.\033[0m\n");
    exit(DEFAULT_ERROR_CODE);
}

// Code 3
void jakarta_error_unknown_symbol(const char symbol) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_3\n");
    printf("Compiler Error: Symbol %c not recognized.\033[0m\n", symbol);
    exit(DEFAULT_ERROR_CODE);
}

// Code 4
void jakarta_error_cannot_close_file(const char* file_name) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_4\n");
    printf("I/O Error: Could not close file %s.\033[0m\n", file_name);
    exit(DEFAULT_ERROR_CODE);
}

// Code 5
void jakarta_error_invalid_token(const char* expected, const char* got) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_5\n");
    printf("Invalid Token Error: Expected token %s but got %s\033[0m\n", expected, got);
    exit(DEFAULT_ERROR_CODE);
}

// Code 6
void jakarta_error_undefined_identifier(const char* identifier) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_6\n");
    printf("Undefined Identifier: Could not find identifier %s\033[0m\n", identifier);
    exit(DEFAULT_ERROR_CODE);
}

// Code 7
void jakarta_error_invalid_typedef_location() {
    printf("\033[31mThere was an error while running your code: ERR_CODE_7\n");
    printf("Typedef statement cannot be used outside of global context\033[0m\n");
    exit(DEFAULT_ERROR_CODE);
}