#include "data_structures/compiler_state.h"
#include "core.h"

CompilerState* create_compiler_state() {
    CompilerState* state = malloc(sizeof(CompilerState));
    if (!state)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "CompilerState");
    state->forest = create_hashmap();
    state->files_to_parse = create_array(10);
    return state;
}