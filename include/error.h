#ifndef __JAKARTA_ERROR_H__
#define __JAKARTA_ERROR_H__

void jakarta_error_no_file_location(const char* prefix);
void jakarta_error_file_does_not_exist(const char* file_name);
void jakarta_error_unknown_symbol(const char symbol);
void jakarta_error_cannot_close_file(const char* file_name);

void jakarta_error_invalid_token(const char* expected, const char* got);

#endif