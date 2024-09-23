#include <stdlib.h>

#include "tokenizer.h"

Tokenizer* create_tokenizer(unsigned int initial_size) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->tokens = malloc(sizeof(Token*) * initial_size);
    tokenizer->max_token_length = initial_size;
    tokenizer->current_token_length = 0;
    return tokenizer;
}