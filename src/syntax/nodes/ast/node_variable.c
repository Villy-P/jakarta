#include <stddef.h>
#include <stdio.h>

#include "data_structures/ast.h"
#include "data_structures/tokenizer.h"
#include "data_structures/array.h"
#include "data_structures/stack.h"
#include "data_structures/symbol_table.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "types.h"
#include "syntax.h"
#include "core.h"

void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    ASTNode* variable_node = parse_variable_declaration(tokenizer, NULL, state);
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

ASTNode* parse_variable_declaration(Tokenizer* tokenizer, FunctionDefinition* function_definition, CompilerState* state) {
    bool is_array = false;

    Token* type_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);

    if (peek(tokenizer, OPERATOR_ARRAY_DECLARATION)) {
        consume(tokenizer);
        is_array = true;
    }

    Token* name_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);

    ASTNode* variable_type_node = create_ast_node(AST_IDENTIFIER_VARIABLE_TYPE, type_token);
    ASTNode* variable_node = create_ast_node(AST_IDENTIFIER_VARIABLE_DEFINITION, name_token);

    Variable* variable = create_variable(name_token->content, type_token->content, is_array);

    add_to_array(variable_node->nodes, variable_type_node);

    if (function_definition != NULL)
        add_to_array(function_definition->parameters, variable);

    return variable_node;
}






void resolve_variable_definition(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving variable definition: %s", node->token->content);

    ASTNode* variable_type_node = (ASTNode*)get_from_array(node->nodes, 0);
    SymbolTableEntry* type_entry = lookup_type(variable_type_node->token->content, symbol_table, state);
    if (type_entry == NULL)
        return handle_error(
            ERROR_UNDEFINED_TYPE, 
            variable_type_node->token, 
            state, 
            variable_type_node->token->content);
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved variable type: %s", type_entry->name);

    ASTNode* variable_content_node = (ASTNode*)get_from_array(node->nodes, 1);
    if (variable_content_node->nodes->length == 0)
        return;
    ASTNode* expression_node = (ASTNode*)get_from_array(variable_content_node->nodes, 0);
    resolve_expression(expression_node, symbol_table, state);
    // TODO: check to see if it matches the type
}