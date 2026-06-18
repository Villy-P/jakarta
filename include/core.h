#pragma once

#include "syntax.h"
#include "data_structures/tokenizer.h"
#include "data_structures/compiler_state.h"

#ifdef __WIN32
#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#endif

typedef enum {
    ERROR_INVALID_FILE_LOCATION,
    ERROR_INVALID_FILE_NAME,
} ErrorCode;

#define ERR_INVALID_FILE_LOCATION 0
#define ERR_INVALID_FILE_NAME 1
#define ERR_UNKNOWN_SYMBOL 2
#define ERR_CANNOT_CLOSE_FILE 3
#define ERR_INVALID_TOKEN 4
#define ERR_UNDEFINED_IDENTIFIER 5
#define ERR_INVALID_TYPEDEF_LOCATION 6
#define ERR_DUPLICATE_IDENTIFIER 7
#define ERR_CUSTOM 8
#define ERR_MALLOC_FAIL 9
#define ERR_UNTERMINATED_STRING 10

// error.c
void jakarta_error(int error_code, Token* token, const char* additional_info);
void jakarta_error_invalid_token(const char* expected, const char* got);
void jakarta_error_invalid_typedef_location(Token* token);
void jakarta_error_undefined_identifier(Token* identifier);

void handle_error(ErrorCode error_code, Token* token, CompilerState* state, const char* text);

// stack_trace.c
void print_stack_trace_old();

// free.c
void free_tokenizer(Tokenizer* tokenizer);
void free_token(Token* token);
void free_type(Type* type);
