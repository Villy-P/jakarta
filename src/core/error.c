#include <ctrace/ctrace.h>
#include <excpt.h>
#include <minwindef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <winnt.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "debug.h"
#include "syntax.h"

static const int DEFAULT_ERROR_CODE = 1;
static const int MAX_CTRACE_DEPTH = 32;
static const int MAX_ERROR_COUNT = 20;

void jakarta_error(int32_t error_code, Token* token,
                   const char* additional_info) {
    printf("\033[31m");
    if (token != NULL) {
        printf("%s:%d:%d: Error: ", token->file_name, token->line, token->col);
    } else {
        printf("Error: ");
    }
    printf("ERR_CODE_%d\n", error_code);
    switch (error_code) {
        // additional_info: the symbol
        case ERR_UNKNOWN_SYMBOL:
            printf("Unknown symbol encountered: %s.\n", additional_info);
            break;
        // additional_info: the token that was expected
        case ERR_INVALID_TOKEN:
            if (token == NULL) {
                printf(
                    "Invalid token encountered, but no token was provided.\n");
                break;
            }
            printf("Invalid token encountered: %d, expected %s.\n",
                   token->symbol, additional_info);
            break;
        // additional_info: the file name that could not be closed
        case ERR_CANNOT_CLOSE_FILE:
            printf("Could not close file %s.\n", additional_info);
            break;
        // additional_info: the token that was not defined
        case ERR_UNDEFINED_IDENTIFIER:
            printf("Identifier %s does not exist in current scope.\n",
                   additional_info);
            break;
        // additional_info: the token that was duplciate
        case ERR_DUPLICATE_IDENTIFIER:
            printf("Identifier %s already exists in current scope.\n",
                   additional_info);
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
            if (token != NULL) {
                printf("%s:%d:%d: %s\n", token->file_name, token->line,
                       token->col, token->content);
            } else {
                printf("An error occurred.\n");
            }
    }
    ctrace_stacktrace trace = ctrace_generate_trace(0, MAX_CTRACE_DEPTH);
    ctrace_print_stacktrace(&trace, stdout, 1);

    ctrace_free_stacktrace(&trace);
    printf("\033[0m\n");
    _Exit(error_code);
}

// Code 5
void jakarta_error_invalid_token(const char* expected, const char* got) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_5\n");
    printf("Invalid Token Error: Expected token %s but got %s\033[0m\n",
           expected, got);
    ctrace_stacktrace trace = ctrace_generate_trace(0, MAX_CTRACE_DEPTH);
    ctrace_print_stacktrace(&trace, stdout, 1);

    ctrace_free_stacktrace(&trace);
    _Exit(DEFAULT_ERROR_CODE);
}

// Code 6
void jakarta_error_undefined_identifier(Token* identifier) {
    printf(
        "\033[31mThere was an error while running your code at position "
        "%s:%d:%d: ERR_CODE_6\033[0m\n",
        identifier->file_name, identifier->line, identifier->col);
    printf("Undefined Identifier: Could not find identifier %s\033[0m\n",
           identifier->content);
    _Exit(DEFAULT_ERROR_CODE);
}

// Code 7
void jakarta_error_invalid_typedef_location(Token* token) {
    printf(
        "\033[31mThere was an error while running your code at position "
        "%s:%d:%d: ERR_CODE_7\033[0m\n",
        token->file_name, token->line, token->col);
    printf(
        "Typedef statement cannot be used outside of global context\033[0m\n");
    _Exit(DEFAULT_ERROR_CODE);
}

void handle_error(int32_t error_code, Token* token, CompilerState* state, ...) {
    va_list args;  // NOLINT
    va_start(args, state);
    log_msg(logs.main, "[ERROR] Handling error with code %d", error_code);
    printf("\033[31m");
    if (token != nullptr) {
        printf("%s:%d:%d: ", token->file_name, token->line, token->col);
    }
    if (error_code & ERROR_FLAG_INTERNAL) {
        // an internal error with the compiler itself. print stack trace and
        // exit with the error code
        printf("Internal Compiler Error: ");
        printf("ERR_CODE_%d\n", error_code);
        switch (error_code) {
            case ERROR_INVALID_FILE_LOCATION: {
                const char* text = va_arg(args, const char*);
                printf("No file argument found for %s.\n", text);
                printf(
                    "Enter a file name or location after %s in your compiler "
                    "args.\n",
                    text);
                break;
            }
            case ERROR_INVALID_FILE_NAME: {
                const char* file_name = va_arg(args, const char*);
                printf("File %s does not exist.\n", file_name);
                printf("Enter a correct file name after -f.\n");
                break;
            }
            default:
                printf("An internal compiler error occurred.\n");
        }
        ctrace_stacktrace trace = ctrace_generate_trace(0, MAX_CTRACE_DEPTH);
        ctrace_print_stacktrace(&trace, stdout, 1);

        ctrace_free_stacktrace(&trace);
        printf("\033[0m\n");
        abort();
    }

    printf("Error: ");
    printf("ERR_CODE_%d\n", error_code);
    switch (error_code) {
        case ERROR_DUPLICATE_IDENTIFIER: {
            if (token == nullptr) {
                printf(
                    "Duplicate identifier error occurred, but no token was "
                    "provided.\n");
                break;
            }
            ASTNode* duplicate_identifier = va_arg(args, ASTNode*);
            printf("Identifier %s already exists in current scope.\n",
                   token->content);
            if (duplicate_identifier != nullptr) {
                printf("First declared at %s:%d:%d\n",
                       duplicate_identifier->token->file_name,
                       duplicate_identifier->token->line,
                       duplicate_identifier->token->col);
            }
            break;
        }
        case ERROR_UNDEFINED_TYPE: {
            const char* return_type = va_arg(args, const char*);
            printf("Undefined type: %s\n", return_type);
            break;
        }
        case ERROR_UNDEFINED_IDENTIFIER: {
            const char* identifier = va_arg(args, const char*);
            printf("Undefined identifier: %s\n", identifier);
            break;
        }
        case ERROR_MISMATCH_PARAMETER_COUNT: {
            const char* identifier = va_arg(args, const char*);
            unsigned int expected = va_arg(args, unsigned int);
            unsigned int got = va_arg(args, unsigned int);
            printf("Mismatch in function parameter count for function %s: ",
                   identifier);
            printf("Expected %d parameters, got %d parameters.", expected, got);
        }
        default:
            printf("An error occurred.\n");
    }

    state->error_count++;
    if (state->error_count > MAX_ERROR_COUNT) {
        printf("Error max of %d reached. Aborting.\n", MAX_ERROR_COUNT);
        _Exit(DEFAULT_ERROR_CODE);
    }
    va_end(args);
    printf("\033[0m\n");
}

long WINAPI handle_seg_fault(EXCEPTION_POINTERS* exception_pointers) {
    if (exception_pointers->ExceptionRecord->ExceptionCode ==
        EXCEPTION_ACCESS_VIOLATION) {
        printf(
            "\033[31mSegmentation Fault (Access Violation) detected at address "
            "%p\033[0m\n",
            exception_pointers->ExceptionRecord->ExceptionAddress);
        ctrace_stacktrace trace = ctrace_generate_trace(0, MAX_CTRACE_DEPTH);
        ctrace_print_stacktrace(&trace, stdout, 1);

        ctrace_free_stacktrace(&trace);
        _Exit(DEFAULT_ERROR_CODE);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}