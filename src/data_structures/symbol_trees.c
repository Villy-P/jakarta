#include "data_structures/symbol_table.h"
#include "data_structures/hashmap.h"
#include "core.h"
#include "debug.h"
#include <string.h>

static HashMap* global_symbol_tree;

void symbol_table_init(void) {
    global_symbol_tree = create_hashmap();
}

void add_symbol_tree_token(Token* token, SymbolType type, void* data, CompilerState* state) {
    void* existing_node = get(global_symbol_tree, token->content);
    if (existing_node != NULL)
        return handle_error(ERROR_DUPLICATE_IDENTIFIER, token, state, (ASTNode*)existing_node);
    insert(global_symbol_tree, token->content, data);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s\n", token->content);
}

void add_symbol_tree_entry(const char* name, SymbolType type, void* data) {
    if (get(global_symbol_tree, name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, name);
    insert(global_symbol_tree, name, data);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s\n", name);
}

void get_symbol_tree_entry(const char* name, SymbolTableEntry* target) {
    void* data = get(global_symbol_tree, name);
    if (data == NULL)
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, NULL, name);
    memcpy(target, data, sizeof(SymbolTableEntry));
}