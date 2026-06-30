#include <stdlib.h>

#include "core.h"
#include "data_structures/tokenizer.h"
#include "syntax.h"
#include "types.h"

void free_tokenizer(Tokenizer* tokenizer) {
    ds_array_destroy(&tokenizer->tokens);
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
