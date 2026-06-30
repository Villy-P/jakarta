#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/hashmap.h"
#include "data_structures/symbol_table.h"
#include "data_structures/tokenizer.h"
#include "libds-c.h"
#include "symbol.h"
#include "syntax.h"

Tokenizer* create_tokenizer(size_t initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    if (tokenizer == nullptr) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Tokenizer");
    }
    if (!ds_array_init(&tokenizer->tokens, initial_size)) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Tokenizer tokens array");
    }
    return tokenizer;
}

void print_tokens(Tokenizer* tokenizer) {
    for (size_t i = 0; i < tokenizer->tokens.length; i++) {
        Token* token = DSM_ARRAY_GET(&tokenizer->tokens, i, Token*);
        printf(
            "Token #%.2zu: %10s at %d:%d, with symbol %d\n", i, 
            token->content, 
            token->line, 
            token->col, 
            token->symbol);
    }
}

Token* consume(Tokenizer* tokenizer) {
    Token* content = DSM_ARRAY_GET(&tokenizer->tokens, 0, Token*);
    ds_array_remove(&tokenizer->tokens, 0);
    return content;
}

bool peek(Tokenizer* tokenizer, Symbol symbol) {
    Token* token = DSM_ARRAY_GET(&tokenizer->tokens, 0, Token*);
    return token->symbol == symbol;
}

bool peek_ahead(Tokenizer* tokenizer, Symbol symbol, size_t offset) {
    if (offset >= tokenizer->tokens.length) {
        return false;
    }
    Token* token = DSM_ARRAY_GET(&tokenizer->tokens, offset, Token*);
    return token->symbol == symbol;
}

bool peek_type(Tokenizer* tokenizer) {
    if (tokenizer->tokens.length == 0) {
        return false;
    }
    Token* token = DSM_ARRAY_GET(&tokenizer->tokens, 0, Token*);
    return (token->symbol == SYMBOL_IDENTIFIER);
}

Token* peek_consume(Tokenizer* tokenizer, Symbol symbol) {
    if (!peek(tokenizer, symbol)) {
        char* expected = get_string_from_symbol(symbol);
        Token* token = DSM_ARRAY_GET(&tokenizer->tokens, 0, Token*);
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
    ds_array* write_parameters = ds_array_create(1);
    ds_array_push(write_parameters, "char");

    FunctionRegistryEntry* write_entry = create_function_registry_entry("void", write_parameters, create_dummy_function_node("write"));

    add_symbol_tree_entry(create_symbol_table_entry("write", SYMBOL_BUILTIN_FUNCTION), state->symbol_tree);
    insert(state->function_registry, "write", write_entry);
}
