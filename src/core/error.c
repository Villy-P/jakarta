#include <stdlib.h>
#include <stdio.h>
#include <ctrace/ctrace.h>

#include "core.h"
#include "debug.h"

#define DEFAULT_ERROR_CODE 1

void jakarta_error(int error_code, Token* token, const char* additional_info) {
    printf("\033[31m");
    if (token != NULL)
        printf("%s:%d:%d: Error: ", token->file_name, token->line, token->col);
    else
        printf("Error: ");
    printf("ERR_CODE_%d\n", error_code);
    switch (error_code) {
        // additional_info: the symbol
        case ERR_UNKNOWN_SYMBOL:
            printf("Unknown symbol encountered: %s.\n", additional_info);
            break;
        // additional_info: the token that was expected
        case ERR_INVALID_TOKEN:
            printf("Invalid token encountered: %d, expected %s.\n", token->symbol, additional_info);
            break;
        // additional_info: the file name that could not be closed
        case ERR_CANNOT_CLOSE_FILE:
            printf("Could not close file %s.\n", additional_info);
            break;
        // additional_info: the token that was not defined
        case ERR_UNDEFINED_IDENTIFIER:
            printf("Identifier %s does not exist in current scope.\n", additional_info);
            break;
        // additional_info: the token that was duplciate
        case ERR_DUPLICATE_IDENTIFIER:
            printf("Identifier %s already exists in current scope.\n", additional_info);
            break;
        // additional_info: the custom error message
        case ERR_CUSTOM:
            printf("%s\n", additional_info);
            break;
        // additional_info: what was trying to be allocated
        case ERR_MALLOC_FAIL:
            printf("Failed to allocate memory for %s\n", additional_info);
            break;
        // additional_info: the string that was not closed
        case ERR_UNTERMINATED_STRING:
            printf("Unterminated string literal: %s\n", additional_info);
            break;
        default:
            if (token != NULL)
                printf("%s:%d:%d: %s\n", token->file_name, token->line, token->col, token->content);
            else
                printf("An error occurred.\n");
    }
    ctrace_stacktrace trace = ctrace_generate_trace(0, 32);
    ctrace_print_stacktrace(&trace, stdout, 1);
    
    ctrace_free_stacktrace(&trace);
    printf("\033[0m\n");
    exit(error_code);
}

// Code 5
void jakarta_error_invalid_token(const char* expected, const char* got) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_5\n");
    printf("Invalid Token Error: Expected token %s but got %s\033[0m\n", expected, got);
    ctrace_stacktrace trace = ctrace_generate_trace(0, 32);
    ctrace_print_stacktrace(&trace, stdout, 1);
    
    ctrace_free_stacktrace(&trace);
    exit(DEFAULT_ERROR_CODE);
}

// Code 6
void jakarta_error_undefined_identifier(Token* identifier) {
    printf("\033[31mThere was an error while running your code at position %s:%d:%d: ERR_CODE_6\033[0m\n", identifier->file_name, identifier->line, identifier->col);
    printf("Undefined Identifier: Could not find identifier %s\033[0m\n", identifier->content);
    exit(DEFAULT_ERROR_CODE);
}

// Code 7
void jakarta_error_invalid_typedef_location(Token* token) {
    printf("\033[31mThere was an error while running your code at position %s:%d:%d: ERR_CODE_7\033[0m\n", token->file_name, token->line, token->col);
    printf("Typedef statement cannot be used outside of global context\033[0m\n");
    exit(DEFAULT_ERROR_CODE);
}




void handle_error(int error_code, Token* token, CompilerState* state, ...) {
    va_list args;
    va_start(args, state);
    log_msg(logs.main, "[ERROR] Handling error with code %d\n", error_code);
    printf("\033[31m");
    if (token != NULL)
        printf("%s:%d:%d: ", token->file_name, token->line, token->col);
    if (error_code & ERROR_FLAG_INTERNAL) {
        // an internal error with the compiler itself. print stack trace and exit with the error code
        printf("Internal Compiler Error: ");
        printf("ERR_CODE_%d\n", error_code);
        switch (error_code) {
            case ERROR_INVALID_FILE_LOCATION: {
                const char* text = va_arg(args, const char*);
                printf("No file argument found for %s.\n", text);
                printf("Enter a file name or location after %s in your compiler args.\n", text);
                break;
            }
            case ERROR_INVALID_FILE_NAME: {
                const char* file_name = va_arg(args, const char*);
                printf("File %s does not exist.\n", file_name);
                printf("Enter a correct file name after -f.\n");
                break;
            }
        }
        ctrace_stacktrace trace = ctrace_generate_trace(0, 32);
        ctrace_print_stacktrace(&trace, stdout, 1);
        
        ctrace_free_stacktrace(&trace);
        printf("\033[0m\n");
        abort();
    }

    printf("Error: ");
    printf("ERR_CODE_%d\n", error_code);
    switch (error_code) {
        case ERROR_DUPLICATE_IDENTIFIER: {
            ASTNode* duplicate_identifier = va_arg(args, ASTNode*);
            printf("Identifier %s already exists in current scope.\n", token->content);
            if (duplicate_identifier != NULL)
                printf("First declared at %s:%d:%d\n", duplicate_identifier->token->file_name, duplicate_identifier->token->line, duplicate_identifier->token->col);
            break;
        }
    }

    va_end(args);
    printf("\033[0m\n");
}