#include "data_structures/compiler_state.h"
#include "core.h"

CompilerState* create_compiler_state() {
    CompilerState* state = malloc(sizeof(CompilerState));
    if (!state)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "CompilerState");
    state->forest = create_array(10);
    state->files_to_parse = create_array(10);
    state->error_list = create_array(20);
    state->error_count = 0;
    return state;
}

ForestEntry* create_forest_entry(char* file_path, ASTNode* root) {
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