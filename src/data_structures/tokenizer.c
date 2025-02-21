#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data_structures/tokenizer.h"
#include "free.h"
#include "types/types.h"
#include "error.h"
#include "debug.h"

#define INITIAL_TYPE_SIZE 64
#define INITIAL_TYPE_ALIAS_SIZE 2
#define INITIAL_VARIABLE_SIZE 128

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->tokens = create_array(initial_size);

    tokenizer->variables = malloc(sizeof(Variable*) * INITIAL_VARIABLE_SIZE);
    tokenizer->max_variable_length = INITIAL_VARIABLE_SIZE;
    tokenizer->current_variable_length = 0;

    tokenizer->function_symbol_tree = create_hashmap();
    tokenizer->type_symbol_tree = create_hashmap();

    create_base_types(tokenizer);
    return tokenizer;
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

void add_function(Tokenizer* tokenizer, FunctionDefinition* function_definition) {
    if (get(tokenizer->function_symbol_tree, function_definition->name) != NULL)
        jakarta_error_duplicate_identifier(function_definition->name);
    insert(tokenizer->function_symbol_tree, function_definition->name, function_definition);
}

void print_tokens(Tokenizer* tokenizer) {
    for (unsigned int i = 0; i < tokenizer->tokens->length; i++) {
        Token* token = NULL;
        get_from_array(tokenizer->tokens, i, &token);
        printf(
            "Token #%.2d: %10s at %d:%d, with symbol %d\n", i, 
            token->content, 
            token->line, 
            token->col, 
            token->symbol);
    }
}

Token* consume(Tokenizer* tokenizer) {
    Token* content = NULL;
    get_from_array(tokenizer->tokens, 0, &content);
    remove_from_array(tokenizer->tokens, 0);
    return content;
}

bool peek(Tokenizer* tokenizer, Symbol symbol) {
    Token* token = NULL;
    get_from_array(tokenizer->tokens, 0, &token);
    return token->symbol == symbol;
}

Token* peek_consume(Tokenizer* tokenizer, Symbol symbol) {
    if (!peek(tokenizer, symbol)) {
        char* expected = get_string_from_symbol(symbol);
        Token* token = NULL;
        get_from_array(tokenizer->tokens, 0, &token);
        char* got = token->content;
        jakarta_error_invalid_token(expected, got);
    }
    return consume(tokenizer);
}
