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
    symbol_table->parent = NULL;
    symbol_table->children = create_array(10);
    return symbol_table;
}

SymbolTableEntry* create_symbol_table_entry(const char* name, SymbolType data_type) {
    SymbolTableEntry* entry = malloc(sizeof(SymbolTableEntry));
    if (!entry)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "SymbolTableEntry");
    entry->name = malloc(strlen(name) + 1);
    if (!entry->name)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "SymbolTableEntry name");
    strcpy(entry->name, name);
    entry->data_type = data_type;
    entry->meta_data = NULL;
    return entry;
}

void add_symbol_tree_token(Token* token, SymbolTableEntry* entry, SymbolTable* symbol_table, CompilerState* state) {
    void* existing_node = get(symbol_table->table, token->content);
    if (existing_node != NULL)
        return handle_error(ERROR_DUPLICATE_IDENTIFIER, token, state, (ASTNode*)existing_node);
    insert(symbol_table->table, entry->name, entry);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s", token->content);
}

void add_symbol_tree_entry(SymbolTableEntry* entry, SymbolTable* symbol_table) {
    if (get(symbol_table->table, entry->name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, entry->name);
    insert(symbol_table->table, entry->name, entry);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s", entry->name);
}

SymbolTableEntry* get_symbol_tree_entry(char* name, SymbolTable* symbol_table) {
    return (SymbolTableEntry*)get(symbol_table->table, name);
}