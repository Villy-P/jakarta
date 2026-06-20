#pragma once

#include "data_structures/hashmap.h"
#include "syntax.h"
#include "data_structures/array.h"
#include "types.h"

// forward declarations
typedef struct ASTNodeDef ASTNode;
typedef struct SymbolTableDef SymbolTable;

typedef struct {
    char *file_path;
    ASTNode *root;
} ForestEntry;

typedef struct {
    char* name;
    char* return_type;
    int parameter_count;
    Array* parameter_types;
    ASTNode* body;
} FunctionRegistryEntry;

typedef struct {
    char* name;
    unsigned char bit_size;
    TypeOptions option;
} TypeRegistryEntry;

typedef struct CompilerStateDef {
    Array* forest;
    Array* files_to_parse;

    Array* error_list;
    int error_count;

    SymbolTable* symbol_tree;
    HashMap* type_registry;
    HashMap* function_registry;
} CompilerState;

FunctionRegistryEntry* create_function_registry_entry(const char* name, const char* return_type, int parameter_count, Array* parameter_types, ASTNode* body);
TypeRegistryEntry* create_type_registry_entry(const char* name, unsigned char bit_size, TypeOptions option);
ForestEntry* create_forest_entry(char* file_path, ASTNode* root);
CompilerState* create_compiler_state();

void print_error_list(CompilerState* state);