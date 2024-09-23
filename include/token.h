#ifndef __JAKARTA_TOKEN_H__
#define __JAKARTA_TOKEN_H__

#include "symbol.h"

#define INITIAL_TOKENS_LENGTH 64

typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);

#endif