#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

#include <stdbool.h>

#include "identifier.h"

#define INITIAL_TYPES_LENGTH 64

typedef struct TypeDef {
    char* name;
    unsigned char bit_size;
    bool is_unsigned;
} Type;

extern Identifier** types;
extern unsigned int max_type_length;
extern unsigned int current_type_length;

void create_base_types();
void add_type(Identifier* type);
void create_type(char* name, unsigned char bit_size, bool is_unsigned);

#endif