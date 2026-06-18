#include "data_structures/compiler_state.h"
#include "core.h"

CompilerState* create_compiler_state() {
    CompilerState* state = malloc(sizeof(CompilerState));
    if (!state)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "CompilerState");
    state->forest = create_array(10);
    state->files_to_parse = create_array(10);
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