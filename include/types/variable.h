#pragma once

#include "types/types.h"

typedef struct VariableDef {
    char* name;
    Type* type;
    bool is_array;
} Variable;

Variable* create_variable(const char* name, Type* type, bool is_array);
