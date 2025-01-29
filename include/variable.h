#ifndef __JAKARTA_VARIABLE_H__
#define __JAKARTA_VARIABLE_H__

#include "types/types.h"

typedef struct VariableDef {
    Type* type;
    char* name;
    unsigned short scope;
} Variable;

Variable* create_variable(Type* type, char* name, unsigned short scope);

#endif