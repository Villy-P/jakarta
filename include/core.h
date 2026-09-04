#pragma once

#ifndef __WIN32
#include <bits/types/siginfo_t.h>
#endif
#include "data_structures/compiler_state.h"
#include "data_structures/tokenizer.h"
#include "syntax.h"

#ifdef __WIN32
#include <windows.h>
#endif

#define ERROR_FLAG_INTERNAL (1 << 31)
#define ERROR_FLAG_FATAL (1 << 30)

#define PACK_ERROR(id, count, is_internal, is_fatal)        \
    (((uint32_t)(id)) | (((uint32_t)(count)) << 16) |       \
     ((is_internal) ? (uint32_t)ERROR_FLAG_INTERNAL : 0U) | \
     ((is_fatal) ? (uint32_t)ERROR_FLAG_FATAL : 0U))

#define ERROR_INVALID_FILE_LOCATION PACK_ERROR(0, 1, 1, 1)
#define ERROR_INVALID_FILE_NAME PACK_ERROR(1, 1, 1, 1)

#define ERROR_DUPLICATE_IDENTIFIER PACK_ERROR(0, 1, 0, 0)
#define ERROR_UNDEFINED_TYPE PACK_ERROR(1, 1, 0, 0)
#define ERROR_UNDEFINED_IDENTIFIER PACK_ERROR(2, 1, 0, 0)
#define ERROR_MISMATCH_PARAMETER_COUNT PACK_ERROR(3, 3, 0, 0)

typedef enum {
    ERR_NONE = 0,
    ERR_INVALID_FILE_LOCATION = 1,
    ERR_UNKNOWN_SYMBOL = 2,
    ERR_CANNOT_CLOSE_FILE = 3,
    ERR_INVALID_TOKEN = 4,
    ERR_UNDEFINED_IDENTIFIER = 5,
    ERR_INVALID_TYPEDEF_LOCATION = 6,
    ERR_DUPLICATE_IDENTIFIER = 7,
    ERR_CUSTOM = 8,
    ERR_MALLOC_FAIL = 9,
    ERR_UNTERMINATED_STRING = 10
} ErrCode;

// error.c
void jakarta_error(int32_t error_code, Token* token,
                   const char* additional_info);
void jakarta_error_invalid_token(const char* expected, const char* got);
void jakarta_error_invalid_typedef_location(Token* token);
void jakarta_error_undefined_identifier(Token* identifier);

void handle_error(int32_t error_code, Token* token, CompilerState* state, ...);
#ifdef __WIN32
long WINAPI handle_seg_fault(EXCEPTION_POINTERS* exception_pointers);
#else
void handle_seg_fault(int sig, siginfo_t* info, void* ucontext);
void install_seg_fault_handler(void);
#endif
// free.c
void free_tokenizer(Tokenizer* tokenizer);
void free_token(Token* token);
void free_type(Type* type);
