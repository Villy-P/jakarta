#include <stdlib.h>

#include "parser.h"
#include "error.h"

void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node) {
    consume(tokenizer);
    char* type_alias = peek_consume(tokenizer, SYMBOL_STRING);
    char* type_name = peek_consume(tokenizer, SYMBOL_STRING);
    Type* type = get_type(tokenizer, type_name);
    if (type == NULL)
        jakarta_error_undefined_identifier(type_name);
    TypeAlias* alias = create_type_alias(type_alias, type);
    add_type_alias(tokenizer, alias);
    peek_consume(tokenizer, SYMBOL_SEMICOLON);
}
