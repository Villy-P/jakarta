#include <stdlib.h>

#include "types.h"

Identifier** types = NULL;
unsigned int max_type_length = INITIAL_TYPES_LENGTH;
unsigned int current_type_length = 0;

void create_base_types() {
    if (types == NULL)
        types = malloc(sizeof(Identifier*) * INITIAL_TYPES_LENGTH);
    add_type(create_identifier(TYPE, "char", "", 1));
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