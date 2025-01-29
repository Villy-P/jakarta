#include <stdlib.h>

#include "free.h"
#include "types/types.h"

void free_tokenizer(Tokenizer* tokenizer) {
    for (unsigned short i = 0; i < tokenizer->current_token_length; i++)
        free_token(tokenizer->tokens[i]);
    free(tokenizer);
}

void free_token(Token* token) {
    free(token->content);
    free(token);
}

void free_type(Type* type) {
    free(type->name);
    free(type);
}
