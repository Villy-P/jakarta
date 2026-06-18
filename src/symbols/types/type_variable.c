#include <stdlib.h>
#include <string.h>

#include "types.h"

Variable* create_variable(const char* name, const char* type, bool is_array) {
    Variable* variable = malloc(sizeof(Variable));
    variable->name = malloc(strlen(name) + 1);
    variable->type = malloc(strlen(type) + 1);
    variable->is_array = is_array;
    strcpy(variable->name, name);
    strcpy(variable->type, type);
    return variable;
}