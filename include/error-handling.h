#pragma once

#include "ds/containers.h"
#include "lexer.h"
#include <stddef.h>

typedef enum {
    ERR_INVALID_FILE_LOCATION,
    ERR_INVALID_FILE_NAME,
    ERR_UNEXPECTED_CLI_ARGUMENT,
} ErrorKind;

typedef enum {
    SEVERITY_INTERNAL,
    SEVERITY_ERROR,
    SEVERITY_WARNING
} Severity;

typedef struct CompilerErrorDef {
    ErrorKind kind;
    Severity severity;
    Token* token;   

    union {
        struct {
            const char* arg_name;
        } invalid_file_location;

        struct {
            const char* file_name;
        } invalid_file_name;

        struct {
            const char* arg_name;
        } unexpected_cli_argument;
    } data;
} CompileError;

CompileError* error_invalid_file_location(const char* arg_name);
CompileError* error_invalid_file_name(const char* file_name);
CompileError* error_unexpected_cli_argument(const char* arg_name);

void print_compile_error(const CompileError* error);
void print_all_errors(ds_compile_error_ptr_array* arr);
