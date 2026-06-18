#pragma once

#include "data_structures/hashmap.h"
#include "data_structures/array.h"

typedef struct CompilerStateDef {
    HashMap* forest;
    Array* files_to_parse;
} CompilerState;

CompilerState* create_compiler_state();