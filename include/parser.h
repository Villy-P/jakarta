#ifndef __JAKARTA_PARSER_H__
#define __JAKARTA_PARSER_H__

#include "tokenizer.h"

void read_line(char* line, Tokenizer* tokenizer);
char* get_string(char** line);

#endif