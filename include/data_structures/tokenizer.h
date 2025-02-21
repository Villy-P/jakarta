#ifndef __JAKARTA_TOKENIZER_H__
#define __JAKARTA_TOKENIZER_H__

#include <stdbool.h>

#include "data_structures/hashmap.h"
#include "data_structures/array.h"
#include "types/function.h"
#include "types/types.h"
#include "token.h"

typedef struct TokenizerDef {
    Array* tokens;
    HashMap* function_symbol_tree;
    HashMap* type_symbol_tree;
} Tokenizer;

Tokenizer* create_tokenizer(unsigned int initial_size);
void add_type(Tokenizer* tokenizer, Type* type);
void add_type_alias(Tokenizer* tokenizer, TypeAlias* type_alias);
void add_function(Tokenizer* tokenizer, FunctionDefinition* function_definition);
void print_tokens(Tokenizer* tokenizer);

Token* consume(Tokenizer* tokenizer);
bool peek(Tokenizer* tokenizer, Symbol symbol);
Token* peek_consume(Tokenizer* tokenizer, Symbol symbol);

void create_base_types(Tokenizer* tokenizer);

#endif