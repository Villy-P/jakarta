#include <stdlib.h>

#include "parser.h"
#include "error.h"

void parse_func(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* func_keyword = consume(tokenizer);
    Token* func_type = peek_consume(tokenizer, SYMBOL_STRING);
    Token* func_name = peek_consume(tokenizer, SYMBOL_STRING);
    Type* type = get_type(tokenizer, func_type->content);
    if (type == NULL)
        jakarta_error_undefined_identifier(func_type);
}