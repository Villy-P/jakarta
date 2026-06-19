#pragma once

#include "compiler_state.h"
#include "data_structures/hashmap.h"

typedef enum {
    SYMBOL_TYPE,
    SYMBOL_FUNCTION,
    SYMBOL_CLASS,
    SYMBOL_VARIABLE,

    SYMBOL_BUILTIN_FUNCTION,
    SYMBOL_BUILTIN_TYPE,
} SymbolType;

typedef struct SymbolTableEntryDef {
    char* name;
    SymbolType type;
    void* data;
} SymbolTableEntry;

typedef struct SymbolTableDef {
    HashMap* table;

    struct SymbolTableDef* parent;
    Array* children;
} SymbolTable;

SymbolTable* create_symbol_table();

void add_symbol_tree_token(Token* token, SymbolType type, void* data, SymbolTable* symbol_table, CompilerState* state);
void add_symbol_tree_entry(const char* name, SymbolType type, void* data, SymbolTable* symbol_table);

void get_symbol_tree_entry(const char* name, SymbolTableEntry* target, SymbolTable* symbol_table);