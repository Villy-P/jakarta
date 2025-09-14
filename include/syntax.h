#pragma once

#include "symbol.h"

#define INITIAL_TOKENS_LENGTH 64

// forward declerations
typedef struct TokenizerDef Tokenizer;

// token.c
typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);

// lexer.c
void read_line(char* line, unsigned int line_number, Tokenizer* tokenizer);
char* get_string(char** line);
char* get_string_literal(char** line);