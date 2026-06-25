#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#define FUNCTION_REGISTRY_INITIAL_CAPACITY 10

FunctionDefinition* create_function_definition(const char* name, const char* return_type) {
    FunctionDefinition* function_definition = malloc(sizeof(FunctionDefinition));
    function_definition->name = malloc(strlen(name) + 1);
    function_definition->return_type = malloc(strlen(return_type) + 1);
    strncpy(function_definition->name, name, strlen(name) + 1);
    strncpy(function_definition->return_type, return_type, strlen(return_type) + 1);
    function_definition->parameters = create_array(FUNCTION_REGISTRY_INITIAL_CAPACITY);
    return function_definition;
}
