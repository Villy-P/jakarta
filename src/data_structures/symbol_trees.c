#include "data_structures/symbol_table.h"
#include "data_structures/hashmap.h"
#include "core.h"
#include "debug.h"
#include <string.h>

SymbolTable* create_symbol_table() {
    SymbolTable* symbol_table = malloc(sizeof(SymbolTable));
    if (!symbol_table)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "SymbolTable");
    symbol_table->table = create_hashmap();
    return symbol_table;
}

void add_symbol_tree_token(Token* token, SymbolType type, void* data, SymbolTable* symbol_table, CompilerState* state) {
    void* existing_node = get(symbol_table->table, token->content);
    if (existing_node != NULL)
        return handle_error(ERROR_DUPLICATE_IDENTIFIER, token, state, (ASTNode*)existing_node);
    insert(symbol_table->table, token->content, data);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s\n", token->content);
}

void add_symbol_tree_entry(const char* name, SymbolType type, void* data, SymbolTable* symbol_table) {
    if (get(symbol_table->table, name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, name);
    insert(symbol_table->table, name, data);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s\n", name);
}

void get_symbol_tree_entry(const char* name, SymbolTableEntry* target, SymbolTable* symbol_table) {
    void* data = get(symbol_table->table, name);
    if (data == NULL)
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, NULL, name);
    memcpy(target, data, sizeof(SymbolTableEntry));
}