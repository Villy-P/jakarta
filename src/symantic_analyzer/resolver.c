#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/symbol_table.h"
#include "semantic_analyzer.h"
#include "debug.h"

void resolve_types(CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving types from AST\n");

    for (unsigned int i = 0; i < state->forest->length; ++i) {
        ForestEntry* entry = (ForestEntry*)get_from_array(state->forest, i);
        ASTNode* ast_root = entry->root;
        log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving types from file: %s\n", entry->file_path);

        resolve_node(ast_root, state->symbol_tree, state);
    }
}

void resolve_node(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    switch (node->identifier) {
        case AST_IDENTIFIER_FUNCTION_DEFINITION: {
            log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving function definition: %s\n", node->token->content);

            SymbolTable* function_scope = create_symbol_table();
            add_to_array(symbol_table->children, function_scope);
            function_scope->parent = symbol_table;

            ASTNode* return_type_node = (ASTNode*)get_from_array(node->nodes, 0);
            SymbolTableEntry* return_type_entry = lookup_type(return_type_node->token->content, function_scope, state);
            if (return_type_entry == NULL) {
                handle_error(ERROR_UNDEFINED_RETURN_TYPE, return_type_node->token, state, return_type_node->token->content);
                break;
            }
            log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved return type: %s\n", return_type_entry->name);

            ASTNode* parameters_node = (ASTNode*)get_from_array(node->nodes, 1);
            for (unsigned int i = 0; i < parameters_node->nodes->length; ++i) {
                ASTNode* parameter_node = (ASTNode*)get_from_array(parameters_node->nodes, i);
                ASTNode* parameter_type_node = (ASTNode*)get_from_array(parameter_node->nodes, 0);
                SymbolTableEntry* parameter_type_entry = lookup_type(parameter_type_node->token->content, function_scope, state);
                if (parameter_type_entry == NULL) {
                    handle_error(ERROR_UNDEFINED_FUNCTION_PARAMETER_TYPE, parameter_type_node->token, state, parameter_type_node->token->content);
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
            return;
        }
        default:
            break;
    }

    for (unsigned int i = 0; i < node->nodes->length; ++i) {
        ASTNode* child = (ASTNode*)get_from_array(node->nodes, i);
        resolve_node(child, symbol_table, state);
    }
}

SymbolTableEntry* lookup_type(const char* target, SymbolTable* symbol_table, CompilerState* state) {
    while (symbol_table != NULL) {
        SymbolTableEntry* entry = get_symbol_tree_entry(target, symbol_table);
        if (entry && (entry->type == SYMBOL_TYPE || entry->type == SYMBOL_BUILTIN_TYPE || entry->type == SYMBOL_CLASS))
            return entry;
        symbol_table = symbol_table->parent;
    }
    return NULL;
}