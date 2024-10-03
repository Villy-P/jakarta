#include <stdlib.h>

#include "free.h"
#include "types.h"

void free_tokenizer(Tokenizer* tokenizer) {
    for (unsigned short i = 0; i < tokenizer->current_token_length; i++)
        free_token(tokenizer->tokens[i]);
    for (unsigned short i = 0; i < tokenizer->current_type_length; i++)
        free(tokenizer->types[i]);
    free(tokenizer->types);
    free(tokenizer->tokens);
    free(tokenizer);
}

void free_token(Token* token) {
    free(token->content);
    free(token);
}
