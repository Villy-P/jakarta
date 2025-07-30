#include <stdlib.h>
#include <string.h>

#include "types/variable.h"

Variable* create_variable(const char* name, Type* type, bool isArray) {
    Variable* variable = malloc(sizeof(Variable));
    variable->name = malloc(strlen(name) + 1);
    variable->type = type;
    variable->isArray = isArray;
    strcpy(variable->name, name);
    return variable;
}