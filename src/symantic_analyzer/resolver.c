#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/symbol_table.h"
#include "semantic_analyzer.h"
#include "debug.h"
#include <stdlib.h>

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
            SymbolTableEntry* return_type_entry = lookup_type(return_type_node->token->content, symbol_table, state);
            if (return_type_entry == NULL) {
                handle_error(ERROR_UNDEFINED_RETURN_TYPE, return_type_node->token, state, return_type_node->token->content);
                break;
            } else {
                log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved return type for function %s: %s\n", node->token->content, return_type_entry->name);
            }
            break;
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