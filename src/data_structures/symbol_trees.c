#include "core.h"
#include "data_structures/array.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/hashmap.h"
#include "data_structures/symbol_table.h"
#include "debug.h"
#include "syntax.h"

#include <stdlib.h>
#include <string.h>

static const int INITIAL_SYMBOL_TABLE_CHILDREN_CAPACITY = 10;

SymbolTable* create_symbol_table() {
    SymbolTable* symbol_table = malloc(sizeof(SymbolTable));
    if (!symbol_table) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "SymbolTable");
        return nullptr;
    }
    symbol_table->table = create_hashmap();
    symbol_table->parent = nullptr;
    if (!init_array(&symbol_table->children, INITIAL_SYMBOL_TABLE_CHILDREN_CAPACITY)) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "SymbolTable children array");
        return nullptr;
    }
    return symbol_table;
}

SymbolTableEntry* create_symbol_table_entry(const char* name, SymbolType data_type) {
    SymbolTableEntry* entry = malloc(sizeof(SymbolTableEntry));
    if (!entry) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "SymbolTableEntry");
        return nullptr;
    }
    entry->name = strdup(name);
    entry->data_type = data_type;
    entry->meta_data = nullptr;
    return entry;
}

void add_symbol_tree_token(Token* token, SymbolTableEntry* entry, SymbolTable* symbol_table, CompilerState* state) {
    void* existing_node = get(symbol_table->table, token->content);
    if (existing_node != nullptr) {
        return handle_error(ERROR_DUPLICATE_IDENTIFIER, token, state, (ASTNode*)existing_node);
    }
    insert(symbol_table->table, entry->name, entry);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s", token->content);
}

void add_symbol_tree_entry(SymbolTableEntry* entry, SymbolTable* symbol_table) {
    if (get(symbol_table->table, entry->name) != nullptr) {
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, nullptr, entry->name);
    }
    insert(symbol_table->table, entry->name, entry);
    log_msg(logs.main, "[SYMBOL TABLE] Added symbol: %s", entry->name);
}

SymbolTableEntry* get_symbol_tree_entry(const char* name, SymbolTable* symbol_table) {
    return (SymbolTableEntry*)get(symbol_table->table, name);
}