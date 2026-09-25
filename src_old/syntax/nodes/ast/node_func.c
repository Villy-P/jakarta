#include <stddef.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/container.h"
#include "data_structures/hashmap.h"
#include "data_structures/symbol_table.h"
#include "data_structures/tokenizer.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "symbol.h"
#include "syntax.h"

void parse_func(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM &&
        ast_node->identifier != AST_IDENTIFIER_CLASS_BODY) {
        jakarta_error_invalid_typedef_location(consume(tokenizer));
    }

    Token* func_keyword = consume(tokenizer);
    Token* func_type = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* func_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* func_node =
        create_ast_node(AST_IDENTIFIER_FUNCTION_DEFINITION, func_name);

    ASTNode* parameters_node =
        create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETERS, nullptr);
    ASTNode* body_node = create_ast_node(AST_IDENTIFIER_FUNCTION_BODY, nullptr);
    ASTNode* return_type_node =
        create_ast_node(AST_IDENTIFIER_FUNCTION_RETURN_TYPE, func_type);

    while (!peek(tokenizer, SYMBOL_CLOSE_PARENTHESIS)) {
        Token* parameter_type_token =
            peek_consume(tokenizer, SYMBOL_IDENTIFIER);
        Token* parameter_name_token =
            peek_consume(tokenizer, SYMBOL_IDENTIFIER);
        Token* comma =
            (int)peek(tokenizer, SYMBOL_COMMA) ? consume(tokenizer) : nullptr;

        ASTNode* parameter_node = create_ast_node(
            AST_IDENTIFIER_FUNCTION_PARAMETER, parameter_name_token);
        ASTNode* parameter_type_node = create_ast_node(
            AST_IDENTIFIER_FUNCTION_PARAMETER_TYPE, parameter_type_token);

        ds_astnode_ptr_array_push(parameter_node->nodes, parameter_type_node);
        ds_astnode_ptr_array_push(parameters_node->nodes, parameter_node);

        if (comma != nullptr) {
            free_token(comma);
        }
    }

    Token* close_parenthesis = consume(tokenizer);
    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);

    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE)) {
        parse(tokenizer, body_node, state);
    }

    Token* close_bracket = consume(tokenizer);

    ds_astnode_ptr_array_push(func_node->nodes, return_type_node);
    ds_astnode_ptr_array_push(func_node->nodes, parameters_node);
    ds_astnode_ptr_array_push(func_node->nodes, body_node);
    ds_astnode_ptr_array_push(ast_node->nodes, func_node);

    free_token(func_keyword);
    free_token(open_parenthesis);
    free_token(close_parenthesis);
    free_token(open_brace);
    free_token(close_bracket);
}

TypeRegistryEntry* resolve_function_definition(ASTNode* node,
                                               SymbolTable* symbol_table,
                                               CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving function definition: %s",
            node->token->content);

    SymbolTable* function_scope = create_symbol_table();
    ds_symbol_table_ptr_array_push(&symbol_table->children, function_scope);
    function_scope->parent = symbol_table;

    ASTNode* return_type_node = ds_astnode_ptr_array_get(node->nodes, 0);
    SymbolTableEntry* return_type_entry =
        lookup_type(return_type_node->token->content, function_scope);
    if (return_type_entry == nullptr) {
        handle_error(ERROR_UNDEFINED_TYPE, return_type_node->token, state,
                     return_type_node->token->content);
        return nullptr;
    }
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved return type: %s",
            return_type_entry->name);

    ASTNode* parameters_node = ds_astnode_ptr_array_get(node->nodes, 1);
    for (unsigned int i = 0; i < parameters_node->nodes->length; ++i) {
        ASTNode* parameter_node =
            ds_astnode_ptr_array_get(parameters_node->nodes, i);
        ASTNode* parameter_type_node =
            ds_astnode_ptr_array_get(parameter_node->nodes, 0);
        SymbolTableEntry* parameter_type_entry =
            lookup_type(parameter_type_node->token->content, function_scope);
        if (parameter_type_entry == nullptr) {
            handle_error(ERROR_UNDEFINED_TYPE, parameter_type_node->token,
                         state, parameter_type_node->token->content);
            continue;
        }
        log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved parameter type: %s",
                parameter_type_entry->name);

        add_symbol_tree_token(
            parameter_node->token,
            create_symbol_table_entry(parameter_node->token->content,
                                      SYMBOL_VARIABLE),
            function_scope, state);
    }

    ASTNode* function_body_node = ds_astnode_ptr_array_get(node->nodes, 2);
    for (unsigned int i = 0; i < function_body_node->nodes->length; ++i) {
        ASTNode* child = ds_astnode_ptr_array_get(function_body_node->nodes, i);
        TypeRegistryEntry* entry = resolve_node(child, function_scope, state);

        if (child->identifier == AST_IDENTIFIER_RETURN) {
            log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving return statement");

            bool compatible = are_types_compatible(
                (TypeRegistryEntry*)get(state->type_registry,
                                        return_type_entry->name),
                entry);

            if (!compatible) {
                handle_error(ERROR_INCOMPATIBLE_TYPES, child->token, state,
                             node->token->content, return_type_entry->name,
                             entry->option);
            }
        }
    }

    return (TypeRegistryEntry*)get(state->type_registry,
                                   return_type_entry->name);
}

TypeRegistryEntry* resolve_function_call(ASTNode* node,
                                         SymbolTable* symbol_table,
                                         CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving function call: %s",
            node->token->content);

    SymbolTableEntry* function_entry =
        lookup_function(node->token->content, symbol_table);
    FunctionRegistryEntry* function_definition =
        get(state->function_registry, node->token->content);
    if (function_entry == nullptr || function_definition == nullptr) {
        handle_error(ERROR_UNDEFINED_IDENTIFIER, node->token, state,
                     node->token->content);
        return nullptr;
    }

    unsigned int expected = function_definition->parameter_types->length;
    unsigned int got = node->nodes->length;

    if (expected != got) {
        handle_error(ERROR_MISMATCH_PARAMETER_COUNT, node->token, state,
                     node->token->content, expected, got);
        return nullptr;
    }

    for (unsigned int i = 0; i < node->nodes->length; i++) {
        ASTNode* child_node = ds_astnode_ptr_array_get(node->nodes, i);
        TypeRegistryEntry* type =
            resolve_expression(child_node, symbol_table, state);
        TypeRegistryEntry* expected_type = (TypeRegistryEntry*)get(
            state->type_registry,
            ds_char_ptr_array_get(function_definition->parameter_types, i));

        bool compatible = are_types_compatible(expected_type, type);
        if (!compatible) {
            handle_error(
                ERROR_INCOMPATIBLE_TYPES, child_node->token, state,
                node->token->content,
                ds_char_ptr_array_get(function_definition->parameter_types, i),
                type->option);
        }
    }
    return (TypeRegistryEntry*)get(state->type_registry,
                                   function_definition->return_type);
}
