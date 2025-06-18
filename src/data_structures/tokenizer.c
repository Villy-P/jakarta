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

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->tokens = create_array(initial_size);
    tokenizer->function_symbol_tree = create_hashmap();
    tokenizer->type_symbol_tree = create_hashmap();
    tokenizer->variable_symbol_stack = create_stack(sizeof(HashMap), 16);

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
        Token* token = get_from_array(tokenizer->tokens, i);
        printf(
            "Token #%.2d: %10s at %d:%d, with symbol %d\n", i, 
            token->content, 
            token->line, 
            token->col, 
            token->symbol);
    }
}

Token* consume(Tokenizer* tokenizer) {
    Token* content = get_from_array(tokenizer->tokens, 0);
    remove_from_array(tokenizer->tokens, 0);
    return content;
}

bool peek(Tokenizer* tokenizer, Symbol symbol) {
    Token* token = get_from_array(tokenizer->tokens, 0);
    return token->symbol == symbol;
}

bool peek_type(Tokenizer* tokenizer) {
    if (tokenizer->tokens->length == 0)
        return false;
    Token* token = get_from_array(tokenizer->tokens, 0);
    if (token->symbol != SYMBOL_STRING)
        return false;
    Type* type = (Type*)get(tokenizer->type_symbol_tree, token->content);
    if (type == NULL)
        return false;
    return true;
}

Token* peek_consume(Tokenizer* tokenizer, Symbol symbol) {
    if (!peek(tokenizer, symbol)) {
        char* expected = get_string_from_symbol(symbol);
        Token* token = get_from_array(tokenizer->tokens, 0);
        char* got = token->content;
        jakarta_error_invalid_token(expected, got);
    }
    return consume(tokenizer);
}

void add_scope(Tokenizer* tokenizer) {
    HashMap* new_scope = create_hashmap();
    push_to_stack(tokenizer->variable_symbol_stack, new_scope);
}

void add_variable_to_scope(Tokenizer* tokenizer, Variable* variable) {
    if (tokenizer->variable_symbol_stack->top == -1)
        jakarta_error_invalid_token("No scope available for variable", variable->name);
    HashMap* current_scope = malloc(sizeof(HashMap));
    int d = pop_from_stack(tokenizer->variable_symbol_stack, current_scope);
    printf("Current scope size: %d\n", d);
    printf("Adding variable %s to scope\n", variable->name);
    if (get(current_scope, variable->name) != NULL)
        jakarta_error_duplicate_identifier(variable->name);
    push_to_stack(tokenizer->variable_symbol_stack, current_scope);
}
