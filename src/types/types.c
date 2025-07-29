#include <stdlib.h>
#include <string.h>

#include "types/types.h"
#include "debug.h"
#include "data_structures/tokenizer.h"

#define BIT_SIZE 1
#define BYTE_SIZE 8
#define SHORT_SIZE 16
#define INT_SIZE 32
#define LONG_SIZE 64
#define LLONG_SIZE 128

void create_base_types(Tokenizer* tokenizer) {
    debug_message("Begun Creating Base Types", TOP_LEVEL);
    add_type(tokenizer, create_type("char",    BYTE_SIZE,   UNUM));
    add_type(tokenizer, create_type("bool",    BIT_SIZE,    ONEBIT));
    add_type(tokenizer, create_type("byte",    BYTE_SIZE,   NUM));
    add_type(tokenizer, create_type("short",   SHORT_SIZE,  NUM));
    add_type(tokenizer, create_type("int",     INT_SIZE,    NUM));
    add_type(tokenizer, create_type("long",    LONG_SIZE,   NUM));
    add_type(tokenizer, create_type("llong",   LLONG_MAX,   NUM));
    add_type(tokenizer, create_type("ubyte",   BYTE_SIZE,   UNUM));
    add_type(tokenizer, create_type("ushort",  SHORT_SIZE,  UNUM));
    add_type(tokenizer, create_type("uint",    INT_SIZE,    UNUM));
    add_type(tokenizer, create_type("ulong",   LONG_SIZE,   UNUM));
    add_type(tokenizer, create_type("ullong" , LLONG_SIZE,  UNUM));
    add_type(tokenizer, create_type("float",   INT_SIZE,    NUM_DECIMAL));
    add_type(tokenizer, create_type("double",  LONG_SIZE,   NUM_DECIMAL));
    add_type(tokenizer, create_type("ldouble", LLONG_SIZE,  NUM_DECIMAL));

    add_type(tokenizer, create_type("string", 0, STRING));
    add_type(tokenizer, create_type("void", 0, STRING));
    debug_message("Created Base Types", CREATION);
}

Type* create_type(const char* name, unsigned char bit_size, TypeOptions option) {
    Type* type = malloc(sizeof(Type));
    type->name = malloc(strlen(name) + 1);
    strcpy(type->name, name);
    type->bit_size = bit_size;
    type->option = option;
    return type;
}

TypeAlias* create_type_alias(const char* name, Type* refers_to) {
    TypeAlias* type_alias = malloc(sizeof(TypeAlias));
    type_alias->name = malloc(strlen(name) + 1);
    strcpy(type_alias->name, name);
    type_alias->refers_to = refers_to;
    return type_alias;
}
