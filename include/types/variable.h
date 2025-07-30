#ifndef __JAKARTA_VARIABLE_H__
#define __JAKARTA_VARIABLE_H__

#include "types/types.h"

typedef struct VariableDef {
    char* name;
    Type* type;
    bool isArray;
} Variable;

Variable* create_variable(const char* name, Type* type, bool isArray);

#endif