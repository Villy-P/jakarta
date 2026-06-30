#pragma once

#include "data_structures/hashmap.h"

#include "syntax.h"

#include "types.h"

#include <libds_c.h>
#include <stdint.h>

// forward declarations
typedef struct ASTNodeDef ASTNode;
typedef struct SymbolTableDef SymbolTable;

typedef struct {
    const char *file_path;
    ASTNode *root;
} ForestEntry;

typedef struct {
    char* return_type;
    ds_array* parameter_types;
    ASTNode* body;
} FunctionRegistryEntry;

typedef struct {
    uint8_t bit_size;
    TypeOptions option;
} TypeRegistryEntry;

typedef struct CompilerStateDef {
    ds_array forest;
    ds_array files_to_parse;

    ds_array error_list;
    uint32_t error_count;

    SymbolTable* symbol_tree;
    HashMap* type_registry;
    HashMap* function_registry;
} CompilerState;

FunctionRegistryEntry* create_function_registry_entry(const char* return_type, ds_array* parameter_types, ASTNode* body);
TypeRegistryEntry* create_type_registry_entry(uint8_t bit_size, TypeOptions option);
ForestEntry* create_forest_entry(const char* file_path, ASTNode* root);
CompilerState* create_compiler_state();

FunctionRegistryEntry* create_function_registry_entry_from_astnode(ASTNode* node);

void print_error_list(CompilerState* state);