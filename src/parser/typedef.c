#include <stdlib.h>

#include "parser.h"
#include "error.h"

void parse_typedef(Tokenizer* tokenizer) {
    consume(tokenizer);
    if (!peek(tokenizer, SYMBOL_STRING))
        jakarta_error_invalid_token("IDENTIFIER", tokenizer->tokens[0]->content);
    char* type_alias = consume(tokenizer);
    if (!peek(tokenizer, SYMBOL_STRING))
        jakarta_error_invalid_token("IDENTIFIER", tokenizer->tokens[0]->content);
    char* type_name = consume(tokenizer);
    Type* type = get_type(tokenizer, type_name);
    if (type == NULL)
        jakarta_error_undefined_identifier(type_name);
}
