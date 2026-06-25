#include <stdlib.h>
#include <string.h>

#include "types.h"

Variable* create_variable(const char* name, const char* type, bool is_array) {
    Variable* variable = malloc(sizeof(Variable));
    variable->name = strdup(name);
    variable->type = strdup(type);
    variable->is_array = is_array;
    return variable;
}