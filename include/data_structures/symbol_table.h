#pragma once

#include "compiler_state.h"
#include "data_structures/hashmap.h"
#include "syntax.h"

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
    ASTNode* data;
} SymbolTableEntry;

typedef struct SymbolTableDef {
    HashMap* table;

    struct SymbolTableDef* parent;
    Array* children;
} SymbolTable;

SymbolTable* create_symbol_table();
SymbolTableEntry* create_symbol_table_entry(const char* name, SymbolType type, ASTNode* data);

void add_symbol_tree_token(Token* token, SymbolTableEntry* entry, SymbolTable* symbol_table, CompilerState* state);
void add_symbol_tree_entry(SymbolTableEntry* entry, SymbolTable* symbol_table);

SymbolTableEntry* get_symbol_tree_entry(char* name, SymbolTable* symbol_table);