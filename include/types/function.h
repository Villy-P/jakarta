#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

typedef struct FunctionDefinitionDef {
    char* name;
    char* return_type;
    char** parameters;
    unsigned short parameter_count;
} FunctionDefinition;

#endif