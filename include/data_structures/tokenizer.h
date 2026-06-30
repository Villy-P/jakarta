#pragma once

#include <stdbool.h>

#include "libds-c.h"
#include "syntax.h"

typedef struct TokenizerDef {
    ds_array tokens;
} Tokenizer;

Tokenizer* create_tokenizer(size_t initial_size);
void print_tokens(Tokenizer* tokenizer);

Token* consume(Tokenizer* tokenizer);
bool peek(Tokenizer* tokenizer, Symbol symbol);
bool peek_ahead(Tokenizer* tokenizer, Symbol symbol, size_t offset);
bool peek_type(Tokenizer* tokenizer);
Token* peek_consume(Tokenizer* tokenizer, Symbol symbol);

void create_base_types(CompilerState* state);

void add_built_in_functions(CompilerState* state);