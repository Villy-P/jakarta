#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "data_structures/ast.h"
#include "debug.h"
#include "data_structures/tokenizer.h"
#include "data_structures/symbol_table.h"

#define BIT_SIZE 1
#define BYTE_SIZE 8
#define SHORT_SIZE 16
#define INT_SIZE 32
#define LONG_SIZE 64
#define LLONG_SIZE 128

static ASTNode* create_dummy_type_node(char* name) {
    Token* dummy_token = create_token(SYMBOL_IDENTIFIER, 0, 0, name, "internal");
    return create_ast_node(AST_IDENTIFIER_DUMMY_TYPE, dummy_token);
}

void create_base_types(CompilerState* state) {
    log_msg(logs.main, "[TYPES] Adding base types to symbol table\n");
    add_symbol_tree_entry(create_symbol_table_entry("char", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("bool", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("byte", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("short", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("int", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("long", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("llong", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("ubyte", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("ushort", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("uint", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("ulong", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("ullong" , SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("float", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("double", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("ldouble", SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("void", SYMBOL_BUILTIN_TYPE), state->symbol_tree);

    log_msg(logs.main, "[TYPES] Adding base types to type registry\n");
    insert(state->type_registry, "char", create_type_registry_entry(BIT_SIZE, ONEBIT));
    insert(state->type_registry, "bool", create_type_registry_entry(BIT_SIZE, ONEBIT));
    insert(state->type_registry, "byte", create_type_registry_entry(BYTE_SIZE, NUM));
    insert(state->type_registry, "short", create_type_registry_entry(SHORT_SIZE, NUM));
    insert(state->type_registry, "int", create_type_registry_entry(INT_SIZE, NUM));
    insert(state->type_registry, "long", create_type_registry_entry(LONG_SIZE, NUM));
    insert(state->type_registry, "llong", create_type_registry_entry(LLONG_SIZE, NUM));
    insert(state->type_registry, "ubyte", create_type_registry_entry(BYTE_SIZE, UNUM));
    insert(state->type_registry, "ushort", create_type_registry_entry(SHORT_SIZE, UNUM));
    insert(state->type_registry, "uint", create_type_registry_entry(INT_SIZE, UNUM));
    insert(state->type_registry, "ulong", create_type_registry_entry(LONG_SIZE, UNUM));
    insert(state->type_registry, "ullong", create_type_registry_entry(LLONG_SIZE, UNUM));
    insert(state->type_registry, "float", create_type_registry_entry(32, NUM_DECIMAL));
    insert(state->type_registry, "double", create_type_registry_entry(64, NUM_DECIMAL));
    insert(state->type_registry, "ldouble", create_type_registry_entry(128, NUM_DECIMAL));
    insert(state->type_registry, "void", create_type_registry_entry(0, NUM));

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
