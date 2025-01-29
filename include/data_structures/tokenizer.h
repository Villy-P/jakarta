#ifndef __JAKARTA_TOKENIZER_H__
#define __JAKARTA_TOKENIZER_H__

#include <stdbool.h>

#include "token.h"
#include "variable.h"
#include "types/types.h"
#include "hashmap.h"

typedef struct TokenizerDef {
    Token** tokens;
    unsigned int max_token_length;
    unsigned int current_token_length;

    Variable** variables;
    unsigned int max_variable_length;
    unsigned int current_variable_length;

    HashMap* function_symbol_tree;
    HashMap* type_symbol_tree;
} Tokenizer;

Tokenizer* create_tokenizer(unsigned int initial_size);
void add_token(Tokenizer* tokenizer, Token* token);
void add_type(Tokenizer* tokenizer, Type* type);
void add_type_alias(Tokenizer* tokenizer, TypeAlias* type_alias);
void add_variable(Tokenizer* tokenizer, Variable* variable);
void print_tokens(Tokenizer* tokenizer);

Token* consume(Tokenizer* tokenizer);
bool peek(Tokenizer* tokenizer, Symbol symbol);
Token* peek_consume(Tokenizer* tokenizer, Symbol symbol);

void create_base_types(Tokenizer* tokenizer);

#endif