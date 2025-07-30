#include <stdlib.h>
#include <string.h>

#include "types/variable.h"

Variable* create_variable(const char* name, Type* type, bool is_array) {
    Variable* variable = malloc(sizeof(Variable));
    variable->name = malloc(strlen(name) + 1);
    variable->type = type;
    variable->is_array = is_array;
    strcpy(variable->name, name);
    return variable;
}