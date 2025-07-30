#ifndef __JAKARTA_TOKENIZER_H__
#define __JAKARTA_TOKENIZER_H__

#include <stdbool.h>

#include "data_structures/hashmap.h"
#include "data_structures/array.h"
#include "data_structures/stack.h"
#include "types/variable.h"
#include "types/function.h"
#include "types/types.h"
#include "token.h"

typedef struct TokenizerDef {
    Array* tokens;
    HashMap* function_symbol_tree;
    HashMap* type_symbol_tree;
    HashMap* class_symbol_tree;
    Stack* variable_symbol_stack;
} Tokenizer;

Tokenizer* create_tokenizer(unsigned int initial_size);
void add_type(Tokenizer* tokenizer, Type* type);
void add_type_alias(Tokenizer* tokenizer, TypeAlias* type_alias);
void add_function(Tokenizer* tokenizer, FunctionDefinition* function_definition);
void print_tokens(Tokenizer* tokenizer);
Variable* get_variable_from_scope(Tokenizer* tokenizer, Token* token);

Token* consume(Tokenizer* tokenizer);
bool peek(Tokenizer* tokenizer, Symbol symbol);
bool peek_type(Tokenizer* tokenizer);
Token* peek_consume(Tokenizer* tokenizer, Symbol symbol);

void create_base_types(Tokenizer* tokenizer);

void add_scope(Tokenizer* tokenizer);
void add_variable_to_scope(Tokenizer* tokenizer, Variable* variable);

void add_built_in_functions(Tokenizer* tokenizer);

#endif