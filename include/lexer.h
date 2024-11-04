#ifndef __JAKARTA_LEXER_H__
#define __JAKARTA_LEXER_H__

#include "tokenizer.h"

void read_line(char* line, unsigned int line_number, Tokenizer* tokenizer);
char* get_string(char** line);

#endif