#include "data_structures/ast.h"
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
        case AST_IDENTIFIER_VARIABLE_DEFINITION: {
            // resolve variable type here
            break;
        }
        case AST_IDENTIFIER_FUNCTION_DEFINITION: {
            // resolve function return type and parameter types here
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