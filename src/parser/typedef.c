#include <stdlib.h>

#include "parser.h"
#include "error.h"

void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* typedef_keyword = consume(tokenizer);
    Token* type_alias = peek_consume(tokenizer, SYMBOL_STRING);
    Token* type_name = peek_consume(tokenizer, SYMBOL_STRING);
    Type* type = get_type(tokenizer, type_name->content);
    if (type == NULL)
        jakarta_error_undefined_identifier(type_name->content);
    TypeAlias* alias = create_type_alias(type_alias, type);
    add_type_alias(tokenizer, alias);
    Token* semicolon = peek_consume(tokenizer, SYMBOL_SEMICOLON);

    ASTNode* typedef_node  = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION, NULL);
    ASTNode* typedef_alias = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION_ALIAS, type_alias);
    ASTNode* typedef_type  = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION_TYPE, type_name);

    add_ast_node(typedef_node, typedef_alias);
    add_ast_node(typedef_node, typedef_type);

    add_ast_node(ast_node, typedef_node);

    free(typedef_keyword);
    free(semicolon);
}
