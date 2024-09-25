#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "free.h"

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->tokens = malloc(sizeof(Token*) * initial_size);
    tokenizer->max_token_length = initial_size;
    tokenizer->current_token_length = 0;
    return tokenizer;
}

void add_token(Tokenizer* tokenizer, Token* token) {
    if (tokenizer->current_token_length <= tokenizer->max_token_length) {
        tokenizer->tokens[tokenizer->current_token_length] = token;
        tokenizer->current_token_length++;
    } else {
        tokenizer->max_token_length *= 2;
        tokenizer->tokens = realloc(tokenizer->tokens, tokenizer->max_token_length * sizeof(Token*));
        tokenizer->tokens[tokenizer->current_token_length] = token;
        tokenizer->current_token_length++;
    }
}

void print_tokens(Tokenizer* tokenizer) {
    for (unsigned int i = 0; i < tokenizer->current_token_length; i++) {
        Token* token = tokenizer->tokens[i];
        printf(
            "Token #%.2d: %10s at %d:%d, with symbol %d\n", i, 
            token->content, 
            token->col, 
            token->line, 
            token->symbol);
    }
}

void consume(Tokenizer* tokenizer) {
    free_token(tokenizer->tokens[0]);
    tokenizer->tokens += sizeof(Token*);
}

bool peek(Tokenizer* tokenizer, Symbol symbol) {
    Token* token = tokenizer->tokens[0];
    return token->symbol == symbol;
}
