// symbol_table.h
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "data_structures/ast.h"
#include "types.h"

void symbol_table_init(void);

void add_type(Type* type);
void add_type_alias(TypeAlias* type_alias);
void add_function(FunctionDefinition* function_definition);
void add_class(ClassDefinition* class_definition);

void get_type(const char* name, Type* target);
void get_function(const char* name, FunctionDefinition* target);
void get_class(const char* name, ClassDefinition* target);

#endif