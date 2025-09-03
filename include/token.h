#pragma once

#include "symbol.h"

#define INITIAL_TOKENS_LENGTH 64

typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);