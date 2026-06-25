#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/tokenizer.h"
#include "core.h"
#include "data_structures/array.h"
#include "data_structures/ast.h"
#include "data_structures/symbol_table.h"


#define INITIAL_TYPE_SIZE 64
#define INITIAL_TYPE_ALIAS_SIZE 2
#define INITIAL_VARIABLE_STACK_SIZE 16

Tokenizer* create_tokenizer(size_t initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    if (tokenizer == nullptr) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Tokenizer");
    }
    if (!init_array(&tokenizer->tokens, initial_size)) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Tokenizer tokens array");
    }
    return tokenizer;
}

void print_tokens(Tokenizer* tokenizer) {
    for (size_t i = 0; i < tokenizer->tokens.length; i++) {
        Token* token = get_from_array(&tokenizer->tokens, i);
        printf(
            "Token #%.2zu: %10s at %d:%d, with symbol %d\n", i, 
            token->content, 
            token->line, 
            token->col, 
            token->symbol);
    }
}

Token* consume(Tokenizer* tokenizer) {
    Token* content = get_from_array(&tokenizer->tokens, 0);
    remove_from_array(&tokenizer->tokens, 0);
    return content;
}

bool peek(Tokenizer* tokenizer, Symbol symbol) {
    Token* token = get_from_array(&tokenizer->tokens, 0);
    return token->symbol == symbol;
}

bool peek_ahead(Tokenizer* tokenizer, Symbol symbol, size_t offset) {
    if (offset >= tokenizer->tokens.length) {
        return false;
    }
    Token* token = get_from_array(&tokenizer->tokens, offset);
    return token->symbol == symbol;
}

bool peek_type(Tokenizer* tokenizer) {
    if (tokenizer->tokens.length == 0) {
        return false;
    }
    Token* token = get_from_array(&tokenizer->tokens, 0);
    return (token->symbol == SYMBOL_IDENTIFIER);
}

Token* peek_consume(Tokenizer* tokenizer, Symbol symbol) {
    if (!peek(tokenizer, symbol)) {
        char* expected = get_string_from_symbol(symbol);
        Token* token = get_from_array(&tokenizer->tokens, 0);
        char* got = token->content;
        jakarta_error_invalid_token(expected, got);
    }
    return consume(tokenizer);
}

static ASTNode* create_dummy_function_node(char* name) {
    Token* dummy_token = create_token(SYMBOL_IDENTIFIER, 0, 0, name, "internal");
    ASTNode* func_node = create_ast_node(AST_IDENTIFIER_DUMMY_FUNCTION, dummy_token);
        
    return func_node;
}

void add_built_in_functions(CompilerState* state) {
    Array* write_parameters = create_array(1);
    add_to_array(write_parameters, "char");

    FunctionRegistryEntry* write_entry = create_function_registry_entry("void", write_parameters, create_dummy_function_node("write"));

    add_symbol_tree_entry(create_symbol_table_entry("write", SYMBOL_BUILTIN_FUNCTION), state->symbol_tree);
    insert(state->function_registry, "write", write_entry);
}
