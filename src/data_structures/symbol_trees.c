#include "data_structures/symbol_table.h"
#include "data_structures/hashmap.h"
#include "core.h"
#include "debug.h"
#include <string.h>

static HashMap* global_function_tree;
static HashMap* global_class_tree;
static HashMap* global_type_tree;

void symbol_table_init(void) {
    global_function_tree = create_hashmap();
    global_class_tree = create_hashmap();
    global_type_tree = create_hashmap();
}

void add_type(Type* type) {
    if (get(global_type_tree, type->name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, type->name);
    insert(global_type_tree, type->name, type);
    log_msg(logs.main, "[TYPES] Added type: %s\n", type->name);
}

void add_type_alias(TypeAlias* type_alias) {
    if (get(global_type_tree, type_alias->name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, type_alias->name);
    insert(global_type_tree, type_alias->name, type_alias->refers_to);
    log_msg(logs.main, "[TYPES] Added type alias: %s -> %s\n", type_alias->name, type_alias->refers_to->name);
}

void add_function(FunctionDefinition* function_definition) {
    if (get(global_function_tree, function_definition->name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, function_definition->name);
    insert(global_function_tree, function_definition->name, function_definition);
    log_msg(logs.main, "[FUNCTIONS] Added function: %s\n", function_definition->name);
}

void add_class(ClassDefinition* class_definition) {
    if (get(global_class_tree, class_definition->name) != NULL)
        jakarta_error(ERR_DUPLICATE_IDENTIFIER, NULL, class_definition->name);
    insert(global_class_tree, class_definition->name, class_definition);
    log_msg(logs.main, "[CLASSES] Added class: %s\n", class_definition->name);
}

void get_type(const char* name, Type* target) {
    Type* type = (Type*)get(global_type_tree, name);
    if (type == NULL)
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, NULL, name);
    memcpy(target, type, sizeof(Type));
}

void get_function(const char* name, FunctionDefinition* target) {
    FunctionDefinition* function_definition = (FunctionDefinition*)get(global_function_tree, name);
    if (function_definition == NULL)
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, NULL, name);
    memcpy(target, function_definition, sizeof(FunctionDefinition));
}

void get_class(const char* name, ClassDefinition* target) {
    ClassDefinition* class_definition = (ClassDefinition*)get(global_class_tree, name);
    if (class_definition == NULL)
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, NULL, name);
    memcpy(target, class_definition, sizeof(ClassDefinition));
}