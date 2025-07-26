#include <stdlib.h>

#include "parser.h"
#include "error.h"
#include "free.h"

void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node) {
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM)
        jakarta_error_invalid_typedef_location(consume(tokenizer));

    Token* typedef_keyword = consume(tokenizer);
    Token* type_alias = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* type_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Type* type = (Type*)get(tokenizer->type_symbol_tree, type_name->content);
    if (type == NULL)
        jakarta_error_undefined_identifier(type_name);
    TypeAlias* alias = create_type_alias(type_alias->content, type);
    add_type_alias(tokenizer, alias);
    Token* semicolon = peek_consume(tokenizer, SYMBOL_SEMICOLON);

    free_token(typedef_keyword);
    free_token(semicolon);
}
