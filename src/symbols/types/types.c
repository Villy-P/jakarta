#include "types.h"

#include "data_structures/compiler_state.h"
#include "data_structures/hashmap.h"
#include "data_structures/symbol_table.h"
#include "data_structures/tokenizer.h"
#include "debug.h"


typedef enum {
    BIT_SIZE = 1,
    BYTE_SIZE = 8,
    SHORT_SIZE = 16,
    INT_SIZE = 32,
    LONG_SIZE = 64,
    LLONG_SIZE = 128
} BitSizes;

// TODO(Valerius Petrini): Unsigned Types
void create_base_types(CompilerState* state) {
    log_msg(logs.main, "[TYPES] Adding base types to symbol table");
    add_symbol_tree_entry(
        create_symbol_table_entry("char", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("bool", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("byte", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("short", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(create_symbol_table_entry("int", SYMBOL_BUILTIN_TYPE),
                          state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("long", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("llong", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    // add_symbol_tree_entry(create_symbol_table_entry("ubyte",
    // SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    // add_symbol_tree_entry(create_symbol_table_entry("ushort",
    // SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    // add_symbol_tree_entry(create_symbol_table_entry("uint",
    // SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    // add_symbol_tree_entry(create_symbol_table_entry("ulong",
    // SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    // add_symbol_tree_entry(create_symbol_table_entry("ullong" ,
    // SYMBOL_BUILTIN_TYPE), state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("float", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("double", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);
    add_symbol_tree_entry(
        create_symbol_table_entry("void", SYMBOL_BUILTIN_TYPE),
        state->symbol_tree);

    log_msg(logs.main, "[TYPES] Adding base types to type registry");
    insert(state->type_registry, "char",
           create_type_registry_entry(BIT_SIZE, ONEBIT));
    insert(state->type_registry, "bool",
           create_type_registry_entry(BIT_SIZE, ONEBIT));
    insert(state->type_registry, "byte",
           create_type_registry_entry(BYTE_SIZE, NUM));
    insert(state->type_registry, "short",
           create_type_registry_entry(SHORT_SIZE, NUM));
    insert(state->type_registry, "int",
           create_type_registry_entry(INT_SIZE, NUM));
    insert(state->type_registry, "long",
           create_type_registry_entry(LONG_SIZE, NUM));
    insert(state->type_registry, "llong",
           create_type_registry_entry(LLONG_SIZE, NUM));
    // insert(state->type_registry, "ubyte",
    // create_type_registry_entry(BYTE_SIZE, UNUM));
    // insert(state->type_registry, "ushort",
    // create_type_registry_entry(SHORT_SIZE, UNUM));
    // insert(state->type_registry, "uint", create_type_registry_entry(INT_SIZE,
    // UNUM)); insert(state->type_registry, "ulong",
    // create_type_registry_entry(LONG_SIZE, UNUM));
    // insert(state->type_registry, "ullong",
    // create_type_registry_entry(LLONG_SIZE, UNUM));
    insert(state->type_registry, "float",
           create_type_registry_entry(INT_SIZE, NUM_DECIMAL));
    insert(state->type_registry, "double",
           create_type_registry_entry(LONG_SIZE, NUM_DECIMAL));
    insert(state->type_registry, "void", create_type_registry_entry(0, NUM));

    log_msg(logs.main, "[TYPES] Success Creating Base Types");
}
