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

    printf("%s -- %s\n", type_alias, type_name);

    ASTNode* typedef_node  = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION, NULL);
    ASTNode* typedef_alias = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION_ALIAS, type_alias);
    ASTNode* typedef_type  = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION_TYPE, type_name);

    add_ast_node(typedef_node, typedef_alias);
    add_ast_node(typedef_node, typedef_type);

    add_ast_node(ast_node, typedef_node);
}
