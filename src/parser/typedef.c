#include "parser.h"
#include "error.h"

void parse_typedef(Tokenizer* tokenizer) {
    consume(tokenizer);
    if (!peek(tokenizer, SYMBOL_STRING))
        jakarta_error_invalid_token("IDENTIFIER", tokenizer->tokens[0]->content);
}
