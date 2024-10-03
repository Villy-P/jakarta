#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "debug.h"
#include "tokenizer.h"

void create_base_types(Tokenizer* tokenizer) {
    debug_message("Begun Creating Base Types", TOP_LEVEL);
    add_type(tokenizer, create_type("char",    8,   UNUM));
    add_type(tokenizer, create_type("bool",    1,   ONEBIT));
    add_type(tokenizer, create_type("byte",    8,   NUM));
    add_type(tokenizer, create_type("short",   16,  NUM));
    add_type(tokenizer, create_type("int",     32,  NUM));
    add_type(tokenizer, create_type("long",    64,  NUM));
    add_type(tokenizer, create_type("llong",   128, NUM));
    add_type(tokenizer, create_type("ubyte",   8,   UNUM));
    add_type(tokenizer, create_type("ushort",  16,  UNUM));
    add_type(tokenizer, create_type("uint",    32,  UNUM));
    add_type(tokenizer, create_type("ulong",   64,  UNUM));
    add_type(tokenizer, create_type("ullong" , 128, UNUM));
    add_type(tokenizer, create_type("float",   32,  DECIMAL));
    add_type(tokenizer, create_type("double",  64,  DECIMAL));
    add_type(tokenizer, create_type("ldouble", 128, DECIMAL));
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