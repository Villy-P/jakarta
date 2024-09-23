#ifndef __JAKARTA_TOKENIZER_H__
#define __JAKARTA_TOKENIZER_H__

#include "token.h"

typedef struct TokenizerDef {
    Token** tokens;
    unsigned int max_token_length;
    unsigned int current_token_length;
} Tokenizer;

Tokenizer* create_tokenizer(unsigned int initial_size);
void add_token(Tokenizer* tokenizer, Token* token);

#endif