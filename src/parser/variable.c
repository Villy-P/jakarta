#include <stddef.h>

#include "data_structures/stack.h"
#include "parser.h"
#include "free.h"
#include "postfix.h"

void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* type_token = peek_consume(tokenizer, SYMBOL_STRING);
    Token* name_token = peek_consume(tokenizer, SYMBOL_STRING);
    Type* type = get(tokenizer->type_symbol_tree, type_token->content);

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* variable_node = create_ast_node(AST_IDENTIFIER_VARIABLE_DEFINITION, name_token);
    ASTNode* variable_type_node = create_ast_node(AST_IDENTIFIER_VARIABLE_TYPE, type_token);
    ASTNode* variable_content_node = create_ast_node(AST_IDENTIFIER_VARIABLE_CONTENT, NULL);

    Variable* variable = create_variable(name_token->content, type);
    add_variable_to_scope(tokenizer, variable);

    add_to_array(variable_content_node->nodes, expression);
    add_to_array(variable_node->nodes, variable_type_node);
    add_to_array(variable_node->nodes, variable_content_node);
    add_to_array(ast_node->nodes, variable_node);

    free_token(type_token);

    consume(tokenizer);
}