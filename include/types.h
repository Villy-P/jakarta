#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

#include <stdbool.h>

#include "identifier.h"

#define INITIAL_TYPES_LENGTH 64

typedef enum TypeOptionsDef {
    NUM,
    UNUM,
    ONEBIT,
    DECIMAL
} TypeOptions;

typedef struct TypeDef {
    char* name;
    unsigned char bit_size;
    TypeOptions option;
} Type;

extern Type** types;
extern unsigned int max_type_length;
extern unsigned int current_type_length;

void create_base_types(void);
void add_type(Type* type);
Type* create_type(const char* name, unsigned char bit_size, TypeOptions option);

#endif