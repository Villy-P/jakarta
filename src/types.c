#include <stdlib.h>

#include "types.h"

Identifier** types = NULL;
unsigned int max_type_length = INITIAL_TYPES_LENGTH;
unsigned int current_type_length = 0;

void create_base_types() {
    if (types == NULL)
        types = malloc(sizeof(Identifier*) * INITIAL_TYPES_LENGTH);
    add_type(create_identifier(TYPE, "char",    "", 1));
    add_type(create_identifier(TYPE, "bool",    "", 1));
    add_type(create_identifier(TYPE, "uchar",   "", 1));
    add_type(create_identifier(TYPE, "short",   "", 2));
    add_type(create_identifier(TYPE, "ushort",  "", 2));
    add_type(create_identifier(TYPE, "int",     "", 4));
    add_type(create_identifier(TYPE, "uint",    "", 4));
    add_type(create_identifier(TYPE, "long",    "", 8));
    add_type(create_identifier(TYPE, "ulong",   "", 8));
    add_type(create_identifier(TYPE, "llong",   "", 16));
    add_type(create_identifier(TYPE, "ullong",  "", 16));
    add_type(create_identifier(TYPE, "float",   "", 4));
    add_type(create_identifier(TYPE, "double",  "", 8));
    add_type(create_identifier(TYPE, "ldouble", "", 16));
}

void add_type(Identifier* type) {
    if (current_type_length <= max_type_length) {
        types[current_type_length] = type;
        current_type_length++;
    } else {
        max_type_length *= 2;
        types = realloc(types, max_type_length * sizeof(Identifier*));
        types[current_type_length] = type;
        current_type_length++;
    }
}

void create_type(char* name, unsigned char bit_size, bool is_unsigned) {
    Type* type = malloc(sizeof(Type));
    type->name = malloc(strlen(name) + 1);
    strcpy(type->name, name);
    type->bit_size = bit_size;
    type->is_unsigned = is_unsigned;
    return type;
}