#include <stdlib.h>
#include <stdio.h>

#include "error.h"

#define DEFAULT_ERROR_CODE 1

void jakarta_error(int error_code, Token* token, const char* additional_info) {
    printf("\033[31m]");
    if (token != NULL)
        printf("Error at position %d:%d: ", token->line, token->col);
    else
        printf("Error: ");
    printf("ERR_CODE_%d\n", error_code);
    switch (error_code) {
        // additional_info: the argument that was missing a file
        case INVALID_FILE_LOCATION:
            printf("Compiler Error: No file argument found for %s.\n", additional_info);
            printf("Enter a file name or location after %s in your compiler args.", additional_info);
            break;
        // additional_info: the file name that was not found
        case INVALID_FILE_NAME:
            printf("Compiler Error: File %s does not exist.\n", additional_info);
            printf("Enter a correct file name after -f.\n");
            break;
        case UNKNOWN_SYMBOL:
            printf("Compiler Error: Unknown symbol encountered: %s.\n", get_string_from_symbol(token->symbol));
            break;
        // additional_info: the file name that could not be closed
        case CANNOT_CLOSE_FILE:
            printf("Compiler Error: Could not close file %s.\n", additional_info);
            break;
        default:
            if (token != NULL)
                printf("Error at position %d:%d: %s\n", token->line, token->col, token->content);
            else
                printf("An error occurred.\n");
    }
    printf("\033[0m\n");
    exit(error_code);
}

// Code 5
void jakarta_error_invalid_token(const char* expected, const char* got) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_5\n");
    printf("Invalid Token Error: Expected token %s but got %s\033[0m\n", expected, got);
    exit(DEFAULT_ERROR_CODE);
}

// Code 6
void jakarta_error_undefined_identifier(Token* identifier) {
    printf("\033[31mThere was an error while running your code at position %d:%d: ERR_CODE_6\n", identifier->line, identifier->col);
    printf("Undefined Identifier: Could not find identifier %s\033[0m\n", identifier->content);
    exit(DEFAULT_ERROR_CODE);
}

// Code 7
void jakarta_error_invalid_typedef_location(Token* token) {
    printf("\033[31mThere was an error while running your code at position %d:%d: ERR_CODE_7\n", token->line, token->col);
    printf("Typedef statement cannot be used outside of global context\033[0m\n");
    exit(DEFAULT_ERROR_CODE);
}

// Code 8
void jakarta_error_duplicate_identifier(char* name) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_8\n");
    printf("Duplicate Identifier: Identifier %s already exists\033[0m\n", name);
    exit(DEFAULT_ERROR_CODE);
}