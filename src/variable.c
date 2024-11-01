#include <string.h>
#include <stdlib.h>

#include "variable.h"

Variable* create_variable(Type* type, char* name, unsigned short scope) {
    Variable* variable = malloc(sizeof(Variable));
    variable->type = malloc(sizeof(Type));
    variable->name = malloc(strlen(name) + 1);
    variable->type = type;
    strcpy(variable->name, name);
    variable->scope = scope;
    return variable;
}