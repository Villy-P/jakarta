#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

#include <stdbool.h>

#include "identifier.h"

typedef enum TypeOptionsDef {
    NUM,
    UNUM,
    ONEBIT,
    NUM_DECIMAL
} TypeOptions;

typedef struct TypeDef {
    char* name;
    unsigned char bit_size;
    TypeOptions option;
} Type;

typedef struct TypeAliasDef {
    char* name;
    Type* refers_to;
} TypeAlias;

Type* create_type(const char* name, unsigned char bit_size, TypeOptions option);
TypeAlias* create_type_alias(const char* name, Type* refers_to);

#endif