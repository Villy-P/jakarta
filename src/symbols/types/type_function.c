#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/array.h"
#include "types.h"

static const int FUNCTION_REGISTRY_INITIAL_CAPACITY = 10;

FunctionDefinition* create_function_definition(const char* name, const char* return_type) {
    FunctionDefinition* function_definition = malloc(sizeof(FunctionDefinition));
    function_definition->name = strdup(name);
    function_definition->return_type = strdup(return_type);
    function_definition->parameters = create_array(FUNCTION_REGISTRY_INITIAL_CAPACITY);
    return function_definition;
}
