#include "data_structures/ast.h"
#include "data_structures/symbol_table.h"
#include "debug.h"
#include "semantic_analyzer.h"


void resolve_types(CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving types from AST");

    for (unsigned int i = 0; i < state->forest.length; ++i) {
        ForestEntry* entry = (ForestEntry*)get_from_array(&state->forest, i);
        ASTNode* ast_root = entry->root;
        log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving types from file: %s", entry->file_path);

        resolve_node(ast_root, state->symbol_tree, state);
    }
}

void resolve_node(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    switch (node->identifier) {
        case AST_IDENTIFIER_FUNCTION_DEFINITION:
            return resolve_function_definition(node, symbol_table, state);
        case AST_IDENTIFIER_VARIABLE_DEFINITION:
            return resolve_variable_definition(node, symbol_table, state);
        default:
            break;
    }

    for (unsigned int i = 0; i < node->nodes->length; ++i) {
        ASTNode* child = (ASTNode*)get_from_array(node->nodes, i);
        resolve_node(child, symbol_table, state);
    }
}

SymbolTableEntry* lookup_type(const char* target, SymbolTable* symbol_table) {
    while (symbol_table != NULL) {
        SymbolTableEntry* entry = get_symbol_tree_entry(target, symbol_table);
        if (entry && (entry->data_type == SYMBOL_TYPE || entry->data_type == SYMBOL_BUILTIN_TYPE || entry->data_type == SYMBOL_CLASS)) {
            return entry;
        }
        symbol_table = symbol_table->parent;
    }
    return NULL;
}

SymbolTableEntry* lookup_function(const char* target, SymbolTable* symbol_table) {
    while (symbol_table != NULL) {
        SymbolTableEntry* entry = get_symbol_tree_entry(target, symbol_table);
        if (entry && (entry->data_type == SYMBOL_FUNCTION || entry->data_type == SYMBOL_BUILTIN_FUNCTION)) {
            return entry;
        }
        symbol_table = symbol_table->parent;
    }
    return NULL;
}