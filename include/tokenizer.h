#ifndef __JAKARTA_TOKENIZER_H__
#define __JAKARTA_TOKENIZER_H__

#include <stdbool.h>

#include "token.h"
#include "types.h"

typedef struct TokenizerDef {
    Token** tokens;
    unsigned int max_token_length;
    unsigned int current_token_length;

    TypeAlias** type_aliases;
    unsigned int max_type_alias_length;
    unsigned int current_type_alias_length;

    Type** types;
    unsigned int max_type_length;
    unsigned int current_type_length;

} Tokenizer;

Tokenizer* create_tokenizer(unsigned int initial_size);
void add_token(Tokenizer* tokenizer, Token* token);
void add_type_alias(Tokenizer* tokenizer, TypeAlias* type_alias);
void print_tokens(Tokenizer* tokenizer);
void consume(Tokenizer* tokenizer);
bool peek(Tokenizer* tokenizer, Symbol symbol);

#endif