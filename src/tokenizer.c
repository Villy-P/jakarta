#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "free.h"
#include "types.h"

#define INITIAL_TYPE_ALIAS_SIZE 20

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->tokens = malloc(sizeof(Token*) * initial_size);
    tokenizer->max_token_length = initial_size;
    tokenizer->current_token_length = 0;

    tokenizer->type_aliases = malloc(sizeof(TypeAlias*) * INITIAL_TYPE_ALIAS_SIZE);
    tokenizer->max_type_alias_length = INITIAL_TYPE_ALIAS_SIZE;
    tokenizer->current_type_alias_length = 0;

    create_base_types(tokenizer);
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

void add_type(Tokenizer* tokenizer, Type* type) {
    if (tokenizer->current_type_length <= tokenizer->max_type_length) {
        tokenizer->types[tokenizer->current_type_length] = type;
        tokenizer->current_type_length++;
    } else {
        tokenizer->max_type_length *= 2;
        tokenizer->types = realloc(tokenizer->types, tokenizer->max_type_length * sizeof(Identifier*));
        tokenizer->types[tokenizer->current_type_length] = type;
        tokenizer->current_type_length++;
    }
}

void add_type_alias(Tokenizer* tokenizer, TypeAlias* type_alias) {
    if (tokenizer->current_type_alias_length <= tokenizer->max_type_alias_length) {
        tokenizer->type_aliases[tokenizer->current_type_alias_length] = type_alias;
        tokenizer->current_type_alias_length++;
    } else {
        tokenizer->max_type_alias_length *= 2;
        tokenizer->type_aliases = realloc(tokenizer->type_aliases, tokenizer->max_type_alias_length * sizeof(Token*));
        tokenizer->type_aliases[tokenizer->current_type_alias_length] = type_alias;
        tokenizer->current_type_alias_length++;
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
