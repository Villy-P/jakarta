#pragma once

#include "data_structures/tokenizer.h"

void read_line(char* line, unsigned int line_number, Tokenizer* tokenizer);
char* get_string(char** line);
char* get_string_literal(char** line);