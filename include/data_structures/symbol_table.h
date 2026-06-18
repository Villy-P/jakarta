// symbol_table.h
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "data_structures/ast.h"
#include "types.h"

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

void symbol_table_init(void);

void add_symbol_tree_entry(const char* name, SymbolType type, void* data);

void get_symbol_tree_entry(const char* name, SymbolTableEntry* target);

#endif