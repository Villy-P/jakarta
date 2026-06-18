#pragma once

#include <stdbool.h>

#include "data_structures/hashmap.h"
#include "data_structures/array.h"
#include "data_structures/stack.h"
#include "types.h"
#include "syntax.h"

typedef struct TokenizerDef {
    Array* tokens;
    Stack* variable_symbol_stack;

    char* current_class;
} Tokenizer;

Tokenizer* create_tokenizer(unsigned int initial_size);
void initialize_symbol_trees(Tokenizer* tokenizer);
void add_class_variable(Tokenizer* tokenizer, Variable* variable);
void add_class_method(Tokenizer* tokenizer, FunctionDefinition* function_definition);
void print_tokens(Tokenizer* tokenizer);
Variable* get_variable_from_scope(Tokenizer* tokenizer, Token* token);

Token* consume(Tokenizer* tokenizer);
bool peek(Tokenizer* tokenizer, Symbol symbol);
bool peek_ahead(Tokenizer* tokenizer, Symbol symbol, unsigned int offset);
bool peek_type(Tokenizer* tokenizer);
Token* peek_consume(Tokenizer* tokenizer, Symbol symbol);

void create_base_types();

void add_scope(Tokenizer* tokenizer);
void add_variable_to_scope(Tokenizer* tokenizer, Variable* variable);

void add_built_in_functions();