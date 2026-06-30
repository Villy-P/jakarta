#define DS_C_IMPLEMENTATION
#include <libds_c.h>

#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/hashmap.h"
#include "data_structures/symbol_table.h"
#include "debug.h"
#include "semantic_analyzer.h"

void gather_declarations(CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Gathering declarations from AST");

    for (unsigned int i = 0; i < state->forest.length; ++i) {
        ForestEntry* entry = DSM_ARRAY_GET(&state->forest, i, ForestEntry*);
        ASTNode* ast_root = entry->root;
        log_msg(logs.main, "[SEMANTIC ANALYZER] Gathering declarations from file: %s", entry->file_path);

        for (unsigned int j = 0; j < ast_root->nodes->length; ++j) {
            ASTNode* node = DSM_ARRAY_GET(ast_root->nodes, j, ASTNode*);
            if (node->identifier == AST_IDENTIFIER_FUNCTION_DEFINITION) {
                add_symbol_tree_token(node->token, create_symbol_table_entry(node->token->content, SYMBOL_FUNCTION), state->symbol_tree, state);
                insert(state->function_registry, node->token->content, create_function_registry_entry_from_astnode(node));
            } else if (node->identifier == AST_IDENTIFIER_CLASS_CREATOR) {
                add_symbol_tree_token(node->token, create_symbol_table_entry(node->token->content, SYMBOL_CLASS), state->symbol_tree, state);
            } else if (node->identifier == AST_IDENTIFIER_TYPE_DEFINITION) {
                add_symbol_tree_token(node->token, create_symbol_table_entry(node->token->content, SYMBOL_TYPE), state->symbol_tree, state);
            } else if (node->identifier == AST_IDENTIFIER_VARIABLE_DEFINITION) {
                add_symbol_tree_token(node->token, create_symbol_table_entry(node->token->content, SYMBOL_VARIABLE), state->symbol_tree, state);
            }
        }
    }
}
 