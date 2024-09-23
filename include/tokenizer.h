#ifndef __JAKARTA_TOKENIZER_H__
#define __JAKARTA_TOKENIZER_H__

#include "token.h"

typedef struct TokenzierDef {
    Token** tokens;
    unsigned int max_token_length;
    unsigned int current_token_length;
} Tokenizer;

#endif