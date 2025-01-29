#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

typedef struct FunctionDefinitionDef {
    char* name;
    char* return_type;
    char** parameters;
    unsigned short parameter_count;
    unsigned short current_parameter;
} FunctionDefinition;

FunctionDefinition* create_function_definition(const char* name, const char* return_type, unsigned short parameter_count);
void add_parameter(FunctionDefinition* function_definition, const char* parameter);

#endif