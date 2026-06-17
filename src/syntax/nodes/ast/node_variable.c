#include <stddef.h>
#include <stdio.h>

#include "data_structures/ast.h"
#include "data_structures/tokenizer.h"
#include "data_structures/array.h"
#include "data_structures/stack.h"
#include "types.h"
#include "syntax.h"
#include "core.h"

void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node) {
    ASTNode* variable_node = parse_variable_declaration(tokenizer, NULL, ast_node);
    if (peek(tokenizer, SYMBOL_SEMICOLON)) {
        consume(tokenizer);
        ASTNode* variable_content_node = create_ast_node(AST_IDENTIFIER_VARIABLE_CONTENT, NULL);
        add_to_array(variable_node->nodes, variable_content_node);
        add_to_array(ast_node->nodes, variable_node);
        return;
    }
    Token* equal_token = peek_consume(tokenizer, SYMBOL_EQUALS);

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* variable_content_node = create_ast_node(AST_IDENTIFIER_VARIABLE_CONTENT, NULL);

    add_to_array(variable_content_node->nodes, expression);
    add_to_array(variable_node->nodes, variable_content_node);
    add_to_array(ast_node->nodes, variable_node);
    
    free_token(equal_token);
}

ASTNode* parse_variable_declaration(Tokenizer* tokenizer, FunctionDefinition* function_definition, ASTNode* class) {
    bool is_array = false;

    Token* type_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);

    if (peek(tokenizer, OPERATOR_ARRAY_DECLARATION)) {
        consume(tokenizer);
        is_array = true;
    }

    Token* name_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);

    Type* type = get(tokenizer->type_symbol_tree, type_token->content);

    ASTNode* variable_type_node = create_ast_node(AST_IDENTIFIER_VARIABLE_TYPE, type_token);
    ASTNode* variable_node = create_ast_node(AST_IDENTIFIER_VARIABLE_DEFINITION, name_token);

    Variable* variable = create_variable(name_token->content, type, is_array);
    add_variable_to_scope(tokenizer, variable);

    if (class->identifier == AST_IDENTIFIER_CLASS_BODY)
        add_class_variable(tokenizer, variable);

    add_to_array(variable_node->nodes, variable_type_node);

    if (function_definition != NULL)
        add_to_array(function_definition->parameters, variable);

    return variable_node;
}