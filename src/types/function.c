#include "function.h"

FunctionDefinition* create_function_definition(const char* name, const char* return_type, unsigned short parameter_count) {
    FunctionDefinition* function_definition = malloc(sizeof(FunctionDefinition));
    function_definition->name = malloc(strlen(name) + 1);
    function_definition->return_type = malloc(strlen(return_type) + 1);
    strcpy(function_definition->name, name);
    strcpy(function_definition->return_type, return_type);
    function_definition->parameters = malloc(sizeof(char*) * parameter_count);
    function_definition->parameter_count = parameter_count;
    function_definition->current_parameter = 0;
    return function_definition;
}

void add_parameter(FunctionDefinition* function_definition, const char* parameter) {
    function_definition->parameters[function_definition->current_parameter] = malloc(strlen(parameter) + 1);
    strcpy(function_definition->parameters[function_definition->current_parameter], parameter);
    function_definition->current_parameter++;
}