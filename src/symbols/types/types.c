#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "debug.h"
#include "data_structures/tokenizer.h"
#include "data_structures/symbol_table.h"

#define BIT_SIZE 1
#define BYTE_SIZE 8
#define SHORT_SIZE 16
#define INT_SIZE 32
#define LONG_SIZE 64
#define LLONG_SIZE 128

void create_base_types() {
    log_msg(logs.main, "[TYPES] Creating base types\n");
    add_symbol_tree_entry("char", SYMBOL_BUILTIN_TYPE, create_type("char",    BYTE_SIZE,   UNUM));
    add_symbol_tree_entry("bool", SYMBOL_BUILTIN_TYPE, create_type("bool",    BIT_SIZE,    ONEBIT));
    add_symbol_tree_entry("byte", SYMBOL_BUILTIN_TYPE, create_type("byte",    BYTE_SIZE,   NUM));
    add_symbol_tree_entry("short", SYMBOL_BUILTIN_TYPE, create_type("short",   SHORT_SIZE,  NUM));
    add_symbol_tree_entry("int", SYMBOL_BUILTIN_TYPE, create_type("int",     INT_SIZE,    NUM));
    add_symbol_tree_entry("long", SYMBOL_BUILTIN_TYPE, create_type("long",    LONG_SIZE,   NUM));
    add_symbol_tree_entry("llong", SYMBOL_BUILTIN_TYPE, create_type("llong",   LLONG_SIZE,  NUM));
    add_symbol_tree_entry("ubyte", SYMBOL_BUILTIN_TYPE, create_type("ubyte",   BYTE_SIZE,   UNUM));
    add_symbol_tree_entry("ushort", SYMBOL_BUILTIN_TYPE, create_type("ushort",  SHORT_SIZE,  UNUM));
    add_symbol_tree_entry("uint", SYMBOL_BUILTIN_TYPE, create_type("uint",    INT_SIZE,    UNUM));
    add_symbol_tree_entry("ulong", SYMBOL_BUILTIN_TYPE, create_type("ulong",   LONG_SIZE,   UNUM));
    add_symbol_tree_entry("ullong" , SYMBOL_BUILTIN_TYPE, create_type("ullong" , LLONG_SIZE,  UNUM));
    add_symbol_tree_entry("float", SYMBOL_BUILTIN_TYPE, create_type("float",   INT_SIZE,    NUM_DECIMAL));
    add_symbol_tree_entry("double", SYMBOL_BUILTIN_TYPE, create_type("double",  LONG_SIZE,   NUM_DECIMAL));
    add_symbol_tree_entry("ldouble", SYMBOL_BUILTIN_TYPE, create_type("ldouble", LLONG_SIZE,  NUM_DECIMAL));

    add_symbol_tree_entry("void", SYMBOL_BUILTIN_TYPE, create_type("void", 0, STRING));
    log_msg(logs.main, "[TYPES] Success Creating Base Types\n");
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
