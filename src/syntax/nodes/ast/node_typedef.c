#include "core.h"
#include "data_structures/ast.h"
#include "syntax.h"

void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM)
        jakarta_error_invalid_typedef_location(consume(tokenizer));

    Token* typedef_keyword = consume(tokenizer);
    Token* type_alias = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* type_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* semicolon = peek_consume(tokenizer, SYMBOL_SEMICOLON);

    ASTNode* typedef_node = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION, type_alias);
    ASTNode* type_node = create_ast_node(AST_IDENTIFIER_TYPE_DEFINITION_TYPE, type_name);

    add_to_array(typedef_node->nodes, type_node);
    add_to_array(ast_node->nodes, typedef_node);

    free_token(typedef_keyword);
    free_token(semicolon);
}
