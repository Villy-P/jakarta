#pragma once

#include "syntax.h"
#include "data_structures/array.h"

// forward declarations
typedef struct ASTNodeDef ASTNode;
typedef struct SymbolTableDef SymbolTable;

typedef struct {
    char *file_path;
    ASTNode *root;
} ForestEntry;

typedef struct CompilerStateDef {
    Array* forest;
    Array* files_to_parse;

    Array* error_list;
    int error_count;

    SymbolTable* symbol_tree;
} CompilerState;

ForestEntry* create_forest_entry(char* file_path, ASTNode* root);
CompilerState* create_compiler_state();

void print_error_list(CompilerState* state);