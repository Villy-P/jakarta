#include <stdio.h>
#include <string.h>

#include "token.h"

Token** tokens = NULL;
unsigned int max_token_length = INITIAL_TOKENS_LENGTH;
unsigned int current_token_length = 0;

void add_token(Token* token) {
    if (current_token_length <= max_token_length) {
        tokens[current_token_length] = token;
        current_token_length++;
    } else {
        max_token_length *= 2;
        tokens = realloc(tokens, max_token_length * sizeof(Token*));
        tokens[current_token_length] = token;
        current_token_length++;
    }
}

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content) {
    Token* item = malloc(sizeof(Token));
    item->symbol = symbol;
    item->line = line;
    item->col = col;
    item->content = malloc(strlen(content) + 1);
    strcpy(item->content, content);
    return item;
}

void print_tokens() {
    for (int i = 0; i < current_token_length; i++) {
        Token* token = tokens[i];
        printf(
            "Token #%.2d: %10s at %d:%d, with symbol %d\n", i, 
            token->symbol != SYMBOL_NEWLINE ? token->content : "NEWLINE", 
            token->col, 
            token->line, 
            token->symbol);
    }
}
