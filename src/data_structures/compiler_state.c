#include "data_structures/compiler_state.h"
#include "data_structures/symbol_table.h"
#include "core.h"

#include <stdio.h>

CompilerState* create_compiler_state() {
    CompilerState* state = malloc(sizeof(CompilerState));
    if (!state)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "CompilerState");
    state->forest = create_array(10);
    state->files_to_parse = create_array(10);
    state->error_list = create_array(20);
    state->error_count = 0;
    state->symbol_tree = create_symbol_table();
    state->type_registry = create_hashmap();
    state->function_registry = create_hashmap();
    return state;
}

ForestEntry* create_forest_entry(const char* file_path, ASTNode* root) {
    ForestEntry* entry = malloc(sizeof(ForestEntry));
    if (!entry)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "ForestEntry");
    entry->file_path = file_path;
    entry->root = root;
    return entry;
}

void print_error_list(CompilerState* state) {
    if (state->error_list->length == 0)
        return;

    printf("Your program has %d unresolved errors:\n", state->error_list->length);
    for (unsigned int i = 0; i < state->error_list->length; ++i) {
        char* error_message = (char*)get_from_array(state->error_list, i);
        printf("%s\n", error_message);
    }
}

TypeRegistryEntry* create_type_registry_entry(unsigned char bit_size, TypeOptions option) {
    TypeRegistryEntry* entry = malloc(sizeof(TypeRegistryEntry));
    if (!entry)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "TypeRegistryEntry");
    entry->bit_size = bit_size;
    entry->option = option;
    return entry;
}

FunctionRegistryEntry* create_function_registry_entry(const char* return_type, Array* parameter_types, ASTNode* body) {
    FunctionRegistryEntry* entry = malloc(sizeof(FunctionRegistryEntry));
    if (!entry)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "FunctionRegistryEntry");
    entry->return_type = malloc(strlen(return_type) + 1);
    if (!entry->return_type)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "FunctionRegistryEntry return type");
    strcpy(entry->return_type, return_type);
    entry->parameter_types = parameter_types;
    entry->body = body;
    return entry;
}

FunctionRegistryEntry* create_function_registry_entry_from_astnode(ASTNode* node) {
    ASTNode* return_type_node = (ASTNode*)get_from_array(node->nodes, 0);
    ASTNode* parameters_node = (ASTNode*)get_from_array(node->nodes, 1);
    ASTNode* body_node = (ASTNode*)get_from_array(node->nodes, 2);

    char* return_type = return_type_node->token->content;
    Array* parameter_types = create_array(parameters_node->nodes->length);
    for (unsigned int i = 0; i < parameters_node->nodes->length; ++i) {
        ASTNode* parameter_node = (ASTNode*)get_from_array(parameters_node->nodes, i);
        ASTNode* parameter_type_node = (ASTNode*)get_from_array(parameter_node->nodes, 0);
        add_to_array(parameter_types, parameter_type_node->token->content);
    }

    return create_function_registry_entry(return_type, parameter_types, body_node);
}