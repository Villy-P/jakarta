#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/hashmap.h"
#include "data_structures/symbol_table.h"
#include "libds_c.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int INITIAL_FOREST_SIZE = 10;
static const int INITIAL_FILES_TO_PARSE_SIZE = 10;
static const int INITIAL_ERROR_LIST_SIZE = 20;

CompilerState* create_compiler_state() {
    CompilerState* state = calloc(1, sizeof(CompilerState));
    if (!state) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "CompilerState");
        return nullptr;
    }
    
    if (!ds_array_init(&state->forest, INITIAL_FOREST_SIZE) ||
        !ds_array_init(&state->files_to_parse, INITIAL_FILES_TO_PARSE_SIZE) ||
        !ds_array_init(&state->error_list, INITIAL_ERROR_LIST_SIZE)) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "CompilerState arrays");
        free(state);
        return nullptr;
    }
    state->error_count = 0;
    state->symbol_tree = create_symbol_table();
    state->type_registry = create_hashmap();
    state->function_registry = create_hashmap();
    return state;
}

ForestEntry* create_forest_entry(const char* file_path, ASTNode* root) {
    ForestEntry* entry = malloc(sizeof(ForestEntry));
    if (!entry) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "ForestEntry");
        return nullptr;
    }
    entry->file_path = strdup(file_path);
    entry->root = root;
    return entry;
}

void print_error_list(CompilerState* state) {
    if (state->error_list.length == 0) {
        return;
    }

    printf("Your program has %zu unresolved errors:\n", state->error_list.length);
    for (unsigned int i = 0; i < state->error_list.length; ++i) {
        char* error_message = DSM_ARRAY_GET(&state->error_list, i, char*);
        printf("%s\n", error_message);
    }
}

TypeRegistryEntry* create_type_registry_entry(unsigned char bit_size, TypeOptions option) {
    TypeRegistryEntry* entry = malloc(sizeof(TypeRegistryEntry));
    if (!entry) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "TypeRegistryEntry");
        return nullptr;
    }
    entry->bit_size = bit_size;
    entry->option = option;
    return entry;
}

FunctionRegistryEntry* create_function_registry_entry(const char* return_type, ds_array* parameter_types, ASTNode* body) {
    FunctionRegistryEntry* entry = malloc(sizeof(FunctionRegistryEntry));
    if (!entry) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "FunctionRegistryEntry");
        return nullptr;
    }
    entry->return_type = strdup(return_type);
    entry->parameter_types = parameter_types;
    entry->body = body;
    return entry;
}

FunctionRegistryEntry* create_function_registry_entry_from_astnode(ASTNode* node) {
    ASTNode* return_type_node = DSM_ARRAY_GET(node->nodes, 0, ASTNode*);
    ASTNode* parameters_node = DSM_ARRAY_GET(node->nodes, 1, ASTNode*);
    ASTNode* body_node = DSM_ARRAY_GET(node->nodes, 2, ASTNode*);

    char* return_type = return_type_node->token->content;
    ds_array* parameter_types = ds_array_create(parameters_node->nodes->length);
    for (unsigned int i = 0; i < parameters_node->nodes->length; ++i) {
        ASTNode* parameter_node = DSM_ARRAY_GET(parameters_node->nodes, i, ASTNode*);
        ASTNode* parameter_type_node = DSM_ARRAY_GET(parameter_node->nodes, 0, ASTNode*);
        ds_array_push(parameter_types, parameter_type_node->token->content);
    }

    return create_function_registry_entry(return_type, parameter_types, body_node);
}