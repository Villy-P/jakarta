#include <stdlib.h>

#include "core.h"
#include "data_structures/tokenizer.h"
#include "syntax.h"

void free_tokenizer(Tokenizer* tokenizer) { free(tokenizer); }

void free_token(Token* token) {
    free(token->content);
    free(token);
}
