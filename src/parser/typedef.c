#include <stdlib.h>

#include "parser.h"
#include "error.h"

void parse_typedef(Tokenizer* tokenizer) {
    consume(tokenizer);
    char* type_alias = peek_consume(tokenizer, SYMBOL_STRING, "IDENTIFIER");
    char* type_name = peek_consume(tokenizer, SYMBOL_STRING, "IDENTIFIER");
    Type* type = get_type(tokenizer, type_name);
    if (type == NULL)
        jakarta_error_undefined_identifier(type_name);
    TypeAlias* alias = create_type_alias(type_alias, type);
    add_type_alias(tokenizer, alias);
    peek_consume(tokenizer, SYMBOL_SEMICOLON, ";");
}
