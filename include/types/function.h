#pragma once

#include "data_structures/ast.h"
#include "data_structures/array.h"

typedef struct FunctionDefinitionDef {
    char* name;
    char* return_type;
    Array* parameters;
    ASTNode* body;
} FunctionDefinition;

FunctionDefinition* create_function_definition(const char* name, const char* return_type);