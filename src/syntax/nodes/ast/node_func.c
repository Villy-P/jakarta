#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "data_structures/ast.h"
#include "data_structures/symbol_table.h"
#include "data_structures/tokenizer.h"
#include "semantic_analyzer.h"
#include "types.h"
#include "types.h"
#include "syntax.h"
#include "core.h"
#include "debug.h"

void parse_func(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM && ast_node->identifier != AST_IDENTIFIER_CLASS_BODY)
        jakarta_error_invalid_typedef_location(consume(tokenizer));

    Token* func_keyword = consume(tokenizer);
    Token* func_type = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* func_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* func_node = create_ast_node(AST_IDENTIFIER_FUNCTION_DEFINITION, func_name);

    ASTNode* parameters_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETERS, NULL);
    ASTNode* body_node = create_ast_node(AST_IDENTIFIER_FUNCTION_BODY, NULL);
    ASTNode* return_type_node = create_ast_node(AST_IDENTIFIER_FUNCTION_RETURN_TYPE, func_type);

    FunctionDefinition* function_definition = create_function_definition(
        func_name->content, 
        func_type->content);

    while (!peek(tokenizer, SYMBOL_CLOSE_PARENTHESIS)) {
        Token* parameter_type_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
        Token* parameter_name_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
        Token* comma = peek(tokenizer, SYMBOL_COMMA) ? consume(tokenizer) : NULL;

        ASTNode* parameter_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETER, parameter_name_token);
        ASTNode* parameter_type_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETER_TYPE, parameter_type_token);

        add_to_array(parameter_node->nodes, parameter_type_node);
        add_to_array(parameters_node->nodes, parameter_node);

        if (comma != NULL)
            free_token(comma);
    }

    Token* close_parenthesis = consume(tokenizer);
    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    Type* type = malloc(sizeof(Type));

    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE))
        parse(tokenizer, body_node, state);

    Token* close_bracket = consume(tokenizer);

    add_to_array(func_node->nodes, return_type_node);
    add_to_array(func_node->nodes, parameters_node);
    add_to_array(func_node->nodes, body_node);
    add_to_array(ast_node->nodes, func_node);

    function_definition->body = malloc(sizeof(ASTNode));
    memcpy(function_definition->body, body_node, sizeof(ASTNode));

    free_token(func_keyword);
    free_token(open_parenthesis);
    free_token(close_parenthesis);
    free_token(open_brace);
    free_token(close_bracket);
}

void parse_func_call(Tokenizer* tokenizer, ASTNode* ast_node, FunctionDefinition* function, CompilerState* state) {
    for (unsigned int i = 0; i < function->parameters->length; i++) {
        Stack* arg = infix_to_postfix(tokenizer);
        ASTNode* arg_node = postfix_to_ast(arg);
        add_to_array(ast_node->nodes, arg_node);
        if (i < function->parameters->length - 1 && peek(tokenizer, SYMBOL_COMMA))
            consume(tokenizer); // comma
    }
    log_msg(logs.main, "[AST] Parsed Function Call\n");
}










void resolve_function_definition(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving function definition: %s\n", node->token->content);

    SymbolTable* function_scope = create_symbol_table();
    add_to_array(symbol_table->children, function_scope);
    function_scope->parent = symbol_table;

    ASTNode* return_type_node = (ASTNode*)get_from_array(node->nodes, 0);
    SymbolTableEntry* return_type_entry = lookup_type(return_type_node->token->content, function_scope, state);
    if (return_type_entry == NULL)
        handle_error(ERROR_UNDEFINED_TYPE, return_type_node->token, state, return_type_node->token->content);
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved return type: %s\n", return_type_entry->name);

    ASTNode* parameters_node = (ASTNode*)get_from_array(node->nodes, 1);
    for (unsigned int i = 0; i < parameters_node->nodes->length; ++i) {
        ASTNode* parameter_node = (ASTNode*)get_from_array(parameters_node->nodes, i);
        ASTNode* parameter_type_node = (ASTNode*)get_from_array(parameter_node->nodes, 0);
        SymbolTableEntry* parameter_type_entry = lookup_type(parameter_type_node->token->content, function_scope, state);
        if (parameter_type_entry == NULL) {
            handle_error(ERROR_UNDEFINED_TYPE, parameter_type_node->token, state, parameter_type_node->token->content);
            continue;
        }
        log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved parameter type: %s\n", parameter_type_entry->name);

        add_symbol_tree_token(parameter_node->token, create_symbol_table_entry(parameter_node->token->content, SYMBOL_VARIABLE, parameter_node), function_scope, state);
    }

    ASTNode* function_body_node = (ASTNode*)get_from_array(node->nodes, 2);
    for (unsigned int i = 0; i < function_body_node->nodes->length; ++i) {
        ASTNode* child = (ASTNode*)get_from_array(function_body_node->nodes, i);
        resolve_node(child, function_scope, state);
    }
}

void resolve_function_call(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving function call: %s\n", node->token->content);

    SymbolTableEntry* function_entry = lookup_function(node->token->content, symbol_table, state);
    if (function_entry == NULL)
        return handle_error(ERROR_UNDEFINED_IDENTIFIER, node->token, state, node->token->content);
}