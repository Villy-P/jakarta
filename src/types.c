#include <stdlib.h>

#include "types.h"

Type** types = NULL;
unsigned int max_type_length = INITIAL_TYPES_LENGTH;
unsigned int current_type_length = 0;

void create_base_types() {
    if (types == NULL)
        types = malloc(sizeof(Identifier*) * INITIAL_TYPES_LENGTH);
    add_type(create_type("char", 8, false));
    add_type(create_type("bool", 1, false));
    add_type(create_type("byte", 8, false));
    add_type(create_type("short", 16, false));
    add_type(create_type("int", 32, false));
    add_type(create_type("long", 64, false));
    add_type(create_type("llong", 128, false));
    add_type(create_type("ubyte", 8, true));
    add_type(create_type("ushort", 16, true));
    add_type(create_type("uint", 32, true));
    add_type(create_type("ulong", 64, true));
    add_type(create_type("ullong", 128, true));
    add_type(create_type("float", 32, false));
    add_type(create_type("double", 64, false));
    add_type(create_type("ldouble", 128, false));
}

void add_type(Type* type) {
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

Type* create_type(char* name, unsigned char bit_size, bool is_unsigned) {
    Type* type = malloc(sizeof(Type));
    type->name = malloc(strlen(name) + 1);
    strcpy(type->name, name);
    type->bit_size = bit_size;
    type->is_unsigned = is_unsigned;
    return type;
}