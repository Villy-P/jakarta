#pragma once

#include "data_structures/ast.h"
#include "data_structures/array.h"
#include "data_structures/hashmap.h"

typedef struct ClassDefinitionDef {
    char* name;
    HashMap* member_variables;
    HashMap* member_functions;
    ASTNode* body;
} ClassDefinition;

ClassDefinition* create_class_definition(const char* name);
