#ifndef __JAKARTA_ERROR_H__
#define __JAKARTA_ERROR_H__

#include "token.h"

#define INVALID_FILE_LOCATION 0
#define INVALID_FILE_NAME 1
#define UNKNOWN_SYMBOL 2
#define CANNOT_CLOSE_FILE 3
#define INVALID_TOKEN 4
#define UNDEFINED_IDENTIFIER 5
#define INVALID_TYPEDEF_LOCATION 6
#define DUPLICATE_IDENTIFIER 7

void jakarta_error(int error_code, Token* token, const char* additional_info);

void jakarta_error_invalid_token(const char* expected, const char* got);
void jakarta_error_undefined_identifier(Token* identifier);

void jakarta_error_invalid_typedef_location(Token* token);
void jakarta_error_duplicate_identifier(char* name);

#endif