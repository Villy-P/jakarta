#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "debug.h"

Type** types = NULL;
unsigned int max_type_length = INITIAL_TYPES_LENGTH;
unsigned int current_type_length = 0;

void create_base_types(void) {
    debug_message("Begun Creating Base Types", TOP_LEVEL);
    if (types == NULL)
        types = malloc(sizeof(Type*) * INITIAL_TYPES_LENGTH);
    add_type(create_type("char",    8,   UNUM));
    add_type(create_type("bool",    1,   ONEBIT));
    add_type(create_type("byte",    8,   NUM));
    add_type(create_type("short",   16,  NUM));
    add_type(create_type("int",     32,  NUM));
    add_type(create_type("long",    64,  NUM));
    add_type(create_type("llong",   128, NUM));
    add_type(create_type("ubyte",   8,   UNUM));
    add_type(create_type("ushort",  16,  UNUM));
    add_type(create_type("uint",    32,  UNUM));
    add_type(create_type("ulong",   64,  UNUM));
    add_type(create_type("ullong" , 128, UNUM));
    add_type(create_type("float",   32,  DECIMAL));
    add_type(create_type("double",  64,  DECIMAL));
    add_type(create_type("ldouble", 128, DECIMAL));
    debug_message("Finished Creating Base Types", TOP_LEVEL);
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

Type* create_type(const char* name, unsigned char bit_size, TypeOptions option) {
    Type* type = malloc(sizeof(Type));
    type->name = malloc(strlen(name) + 1);
    strcpy(type->name, name);
    type->bit_size = bit_size;
    type->option = option;
    return type;
}