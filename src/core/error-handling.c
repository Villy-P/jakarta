#include "error-handling.h"
#include <stdio.h>
#include <stdlib.h>
#include "ds/containers.h"

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

CompileError* error_unexpected_cli_argument(const char* arg_name) {
    CompileError* err = alloc_error(ERR_UNEXPECTED_CLI_ARGUMENT, SEVERITY_ERROR, nullptr);
    err->data.unexpected_cli_argument.arg_name = arg_name;
    return err;
}

static const char* severity_label(Severity sev) {
    switch (sev) {
        case SEVERITY_INTERNAL: return "Internal Compiler Error";
        case SEVERITY_ERROR:    return "Error";
        case SEVERITY_WARNING:  return "Warning";
    }
    return "Unknown";
}

void print_compile_error(const CompileError* error) {
    printf("\033[31m");

    if (error->token != NULL) {
        printf("%s:%d:%d: ", error->token->file_name.data, error->token->line, error->token->col);
    }

    printf("%s: ", severity_label(error->severity));

    switch (error->kind) {
        case ERR_INVALID_FILE_LOCATION:
            printf("No file argument found for %s.\n", error->data.invalid_file_location.arg_name);
            printf("Enter a file name or location after %s in your compiler args.\n",
                   error->data.invalid_file_location.arg_name);
            break;

        case ERR_INVALID_FILE_NAME:
            printf("File %s does not exist.\n", error->data.invalid_file_name.file_name);
            printf("Enter a correct file name after -f.\n");
            break;

        case ERR_UNEXPECTED_CLI_ARGUMENT:
            printf("Unexpected argument %s.\n", error->data.unexpected_cli_argument.arg_name);
            break;
    }

    printf("\033[0m\n");
}

void print_all_errors(ds_compile_error_ptr_array* arr) {
    for (size_t i = 0; i < arr->length; ++i) {
        print_compile_error(ds_compile_error_ptr_array_get(arr, i));
    }
}
