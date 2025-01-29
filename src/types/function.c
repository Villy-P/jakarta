#include "types/function.h"

FunctionDefinition* create_function_definition(const char* name, const char* return_type) {
    FunctionDefinition* function_definition = malloc(sizeof(FunctionDefinition));
    function_definition->name = malloc(strlen(name) + 1);
    function_definition->return_type = malloc(strlen(return_type) + 1);
    strcpy(function_definition->name, name);
    strcpy(function_definition->return_type, return_type);
    return function_definition;
}

Parameter* create_parameter(const char* name, const char* type) {
    Parameter* parameter = malloc(sizeof(Parameter));
    parameter->name = malloc(strlen(name) + 1);
    parameter->type = malloc(strlen(type) + 1);
    strcpy(parameter->name, name);
    strcpy(parameter->type, type);
    return parameter;
}