#pragma once

#include "syntax.h"
#include "data_structures/tokenizer.h"
#include "data_structures/compiler_state.h"

#ifdef __WIN32
#include <windows.h>
#include <dbghelp.h>
#endif

#define ERROR_FLAG_INTERNAL (1 << 31)
#define ERROR_FLAG_FATAL (1 << 30)

#define PACK_ERROR(id, count, is_internal, is_fatal) \
    ((id) | ((count) << 16) | ((is_internal) ? ERROR_FLAG_INTERNAL : 0) | ((is_fatal) ? ERROR_FLAG_FATAL : 0))

#define ERROR_INVALID_FILE_LOCATION PACK_ERROR(0, 1, 1, 1)
#define ERROR_INVALID_FILE_NAME PACK_ERROR(1, 1, 1, 1)

#define ERROR_DUPLICATE_IDENTIFIER PACK_ERROR(0, 1, 0, 0)
#define ERROR_UNDEFINED_TYPE PACK_ERROR(1, 1, 0, 0)

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

void handle_error(int error_code, Token* token, CompilerState* state, ...);
long WINAPI handle_seg_fault(EXCEPTION_POINTERS* exception_pointers);

// free.c
void free_tokenizer(Tokenizer* tokenizer);
void free_token(Token* token);
void free_type(Type* type);
