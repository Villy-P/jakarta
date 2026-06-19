#include <stdlib.h>

#include "semantic_analyzer.h"
#include "debug.h"
#include "data_structures/symbol_table.h"

void gather_declarations(CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Gathering declarations from AST\n");

    for (int i = 0; i < state->forest->length; ++i) {
        ForestEntry* entry = (ForestEntry*)get_from_array(state->forest, i);
        ASTNode* ast_root = entry->root;
        log_msg(logs.main, "[SEMANTIC ANALYZER] Gathering declarations from file: %s\n", entry->file_path);

        for (unsigned int j = 0; j < ast_root->nodes->length; ++j) {
            ASTNode* node = (ASTNode*)get_from_array(ast_root->nodes, j);
            if (node->identifier == AST_IDENTIFIER_FUNCTION_DEFINITION)
                add_symbol_tree_token(node->token, SYMBOL_FUNCTION, node, state);
            else if (node->identifier == AST_IDENTIFIER_CLASS_CREATOR)
                add_symbol_tree_token(node->token, SYMBOL_CLASS, node, state);
            else if (node->identifier == AST_IDENTIFIER_TYPE_DEFINITION)
                add_symbol_tree_token(node->token, SYMBOL_TYPE, node, state);
            else if (node->identifier == AST_IDENTIFIER_VARIABLE_DEFINITION)
                add_symbol_tree_token(node->token, SYMBOL_VARIABLE, node, state);
        }
    }
}