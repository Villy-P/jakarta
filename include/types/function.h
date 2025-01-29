#ifndef __JAKARTA_FUNCTION_H__
#define __JAKARTA_FUNCTION_H__

typedef struct ParameterDef {
    char* name;
    char* type;
} Parameter;

typedef struct FunctionDefinitionDef {
    char* name;
    char* return_type;
    Parameter** parameters;
    unsigned short parameter_count;
    unsigned short current_parameter;
} FunctionDefinition;

FunctionDefinition* create_function_definition(const char* name, const char* return_type);
Parameter* create_parameter(const char* name, const char* type);

#endif