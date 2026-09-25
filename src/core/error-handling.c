#include "error-handling.h"
#include <stdio.h>
#include <stdlib.h>

static CompileError* alloc_error(ErrorKind kind, Severity severity, Token* token) {
    CompileError* err = malloc(sizeof(CompileError));
    if (err == NULL) {
        fprintf(stderr, "Fatal: out of memory allocating CompileError\n");
        abort();
    }
    err->kind = kind;
    err->severity = severity;
    err->token = token;
    return err;
}

CompileError* error_invalid_file_location(const char* arg_name) {
    CompileError* err = alloc_error(ERR_INVALID_FILE_LOCATION, SEVERITY_INTERNAL, nullptr);
    err->data.invalid_file_location.arg_name = arg_name;
    return err;
}

CompileError* error_invalid_file_name(const char* file_name) {
    CompileError* err = alloc_error(ERR_INVALID_FILE_NAME, SEVERITY_INTERNAL, nullptr);
    err->data.invalid_file_name.file_name = file_name;
    return err;
}
