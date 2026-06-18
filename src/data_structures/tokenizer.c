#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data_structures/tokenizer.h"
#include "data_structures/symbol_table.h"
#include "types.h"
#include "core.h"
#include "debug.h"

#define INITIAL_TYPE_SIZE 64
#define INITIAL_TYPE_ALIAS_SIZE 2
#define INITIAL_VARIABLE_STACK_SIZE 16

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    if (tokenizer == NULL)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Tokenizer");
    tokenizer->tokens = create_array(initial_size);
    return tokenizer;
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

bool peek_ahead(Tokenizer* tokenizer, Symbol symbol, unsigned int offset) {
    if (offset >= tokenizer->tokens->length)
        return false;
    Token* token = get_from_array(tokenizer->tokens, offset);
    return token->symbol == symbol;
}

bool peek_type(Tokenizer* tokenizer) {
    if (tokenizer->tokens->length == 0)
        return false;
    Token* token = get_from_array(tokenizer->tokens, 0);
    if (token->symbol != SYMBOL_IDENTIFIER)
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

void add_built_in_functions() {
    FunctionDefinition* write_function = create_function_definition("write", "void");
    Variable* write_param = create_variable("value", "char", false);
    add_to_array(write_function->parameters, write_param);
    add_symbol_tree_entry("write", SYMBOL_BUILTIN_FUNCTION, write_function);
}
