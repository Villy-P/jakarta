#pragma once

#include "lexer.h"
#include <stddef.h>

typedef enum {
    ERR_INVALID_FILE_LOCATION,
    ERR_INVALID_FILE_NAME,
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
    } data;
} CompileError;

CompileError* error_invalid_file_location(const char* arg_name);
CompileError* error_invalid_file_name(const char* file_name);

