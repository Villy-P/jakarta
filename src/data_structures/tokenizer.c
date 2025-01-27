#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data_structures/tokenizer.h"
#include "free.h"
#include "types.h"
#include "error.h"
#include "debug.h"

#define INITIAL_TYPE_SIZE 64
#define INITIAL_TYPE_ALIAS_SIZE 2
#define INITIAL_VARIABLE_SIZE 128

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->tokens = malloc(sizeof(Token*) * initial_size);
    tokenizer->max_token_length = initial_size;
    tokenizer->current_token_length = 0;

    tokenizer->variables = malloc(sizeof(Type*) * INITIAL_VARIABLE_SIZE);
    tokenizer->max_variable_length = INITIAL_VARIABLE_SIZE;
    tokenizer->current_variable_length = 0;

    tokenizer->type_symbol_tree = create_hashmap();

    create_base_types(tokenizer);
    return tokenizer;
}

void add_token(Tokenizer* tokenizer, Token* token) {
    if (tokenizer->current_token_length >= tokenizer->max_token_length) {
        tokenizer->max_token_length *= 2;
        tokenizer->tokens = realloc(tokenizer->tokens, tokenizer->max_token_length * sizeof(Token*));
    }
    tokenizer->tokens[tokenizer->current_token_length] = token;
    tokenizer->current_token_length++;
}

void add_type(Tokenizer* tokenizer, Type* type) {
    if (get(tokenizer->type_symbol_tree, type->name) != NULL)
        jakarta_error_duplicate_identifier(type->name);
    insert(tokenizer->type_symbol_tree, type->name, type);
}

void add_type_alias(Tokenizer* tokenizer, TypeAlias* type_alias) {
    if (get(tokenizer->type_symbol_tree, type_alias->name) != NULL)
        jakarta_error_duplicate_identifier(type_alias->name);
    insert(tokenizer->type_symbol_tree, type_alias->name, type_alias->refers_to);
    debug_message("Added type alias", TOP_LEVEL);
}

void add_variable(Tokenizer* tokenizer, Variable* variable) {
    if (tokenizer->current_variable_length >= tokenizer->max_variable_length) {
        tokenizer->max_variable_length *= 2;
        tokenizer->variables = realloc(tokenizer->variables, tokenizer->max_variable_length * sizeof(Variable*));
    }
    tokenizer->variables[tokenizer->current_variable_length] = variable;
    tokenizer->current_variable_length++;
    debug_message("Added variable", TOP_LEVEL);
}

void print_tokens(Tokenizer* tokenizer) {
    for (unsigned int i = 0; i < tokenizer->current_token_length; i++) {
        Token* token = tokenizer->tokens[i];
        printf(
            "Token #%.2d: %10s at %d:%d, with symbol %d\n", i, 
            token->content, 
            token->line, 
            token->col, 
            token->symbol);
    }
}

Token* consume(Tokenizer* tokenizer) {
    Token* content = tokenizer->tokens[0];
    tokenizer->tokens += 1;
    tokenizer->current_token_length -= 1;
    return content;
}

bool peek(Tokenizer* tokenizer, Symbol symbol) {
    Token* token = tokenizer->tokens[0];
    return token->symbol == symbol;
}

Token* peek_consume(Tokenizer* tokenizer, Symbol symbol) {
    if (!peek(tokenizer, symbol)) {
        char* expected = get_string_from_symbol(symbol);
        char* got = tokenizer->tokens[0]->content;
        jakarta_error_invalid_token(expected, got);
    }
    return consume(tokenizer);
}
