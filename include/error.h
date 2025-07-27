#ifndef __JAKARTA_ERROR_H__
#define __JAKARTA_ERROR_H__

#include "token.h"

#define ERR_INVALID_FILE_LOCATION 0
#define ERR_INVALID_FILE_NAME 1
#define ERR_UNKNOWN_SYMBOL 2
#define ERR_CANNOT_CLOSE_FILE 3
#define ERR_INVALID_TOKEN 4
#define ERR_UNDEFINED_IDENTIFIER 5
#define ERR_INVALID_TYPEDEF_LOCATION 6
#define ERR_DUPLICATE_IDENTIFIER 7
#define ERR_CUSTOM 8

void jakarta_error(int error_code, Token* token, const char* additional_info);

void jakarta_error_invalid_token(const char* expected, const char* got);
void jakarta_error_undefined_identifier(Token* identifier);

void jakarta_error_invalid_typedef_location(Token* token);
void jakarta_error_duplicate_identifier(char* name);

#endif