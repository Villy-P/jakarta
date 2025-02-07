#include <stdlib.h>
#include <string.h>

#include "types/function.h"

FunctionDefinition* create_function_definition(const char* name, const char* return_type) {
    FunctionDefinition* function_definition = malloc(sizeof(FunctionDefinition));
    function_definition->name = malloc(strlen(name) + 1);
    function_definition->return_type = malloc(strlen(return_type) + 1);
    strncpy_s(function_definition->name, strlen(name) + 1, name, strlen(name) + 1);
    strncpy_s(function_definition->return_type, strlen(return_type) + 1, return_type, strlen(return_type) + 1);
    function_definition->current_parameter = 0;
    function_definition->parameter_count = 10;
    function_definition->parameters = malloc(sizeof(Parameter*) * function_definition->parameter_count);
    return function_definition;
}

Parameter* create_parameter(const char* name, const char* type) {
    Parameter* parameter = malloc(sizeof(Parameter));
    parameter->name = malloc(strlen(name) + 1);
    parameter->type = malloc(strlen(type) + 1);
    strncpy_s(parameter->name, strlen(name) + 1, name, strlen(name) + 1);
    strncpy_s(parameter->type, strlen(type) + 1, type, strlen(type) + 1);
    return parameter;
}

void add_parameter(FunctionDefinition* function_definition, Parameter* parameter) {
    if (function_definition->current_parameter >= function_definition->parameter_count) {
        function_definition->parameter_count *= 2;
        function_definition->parameters = realloc(function_definition->parameters, sizeof(Parameter*) * function_definition->parameter_count);
    }
    function_definition->parameters[function_definition->current_parameter] = parameter;
    function_definition->current_parameter++;
}