#ifndef __JAKARTA_ERROR_H__
#define __JAKARTA_ERROR_H__

#include "token.h"

#define INVALID_FILE_LOCATION 0

void jakarta_error(int error_code, Token* token, const char* additional_info);

void jakarta_error_file_does_not_exist(const char* file_name);
void jakarta_error_unknown_symbol(const char symbol);
void jakarta_error_cannot_close_file(const char* file_name);

void jakarta_error_invalid_token(const char* expected, const char* got);
void jakarta_error_undefined_identifier(Token* identifier);

void jakarta_error_invalid_typedef_location(Token* token);
void jakarta_error_duplicate_identifier(char* name);

#endif