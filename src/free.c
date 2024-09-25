#include <stdlib.h>

#include "free.h"
#include "types.h"

void free_type_data(void) {
    for (unsigned short i = 0; i < current_type_length; i++)
        free(types[i]);
    free(types);
}

void free_tokenizer(Tokenizer* tokenizer) {
    for (unsigned short i = 0; i < tokenizer->current_token_length; i++)
        free_token(tokenizer->tokens[i]);
    free(tokenizer);
}

void free_token(Token* token) {
    free(token->content);
    free(token);
}
