#pragma once

#include "data_structures/ast.h"
#include "data_structures/array.h"
#include "data_structures/hashmap.h"

// types.c
typedef enum TypeOptionsDef {
    NUM,
    UNUM,
    ONEBIT,
    NUM_DECIMAL,
    STRING,
    CLASS
} TypeOptions;

typedef struct TypeDef {
    char* name;
    unsigned char bit_size;
    TypeOptions option;
} Type;

typedef struct TypeAliasDef {
    char* name;
    Type* refers_to;
} TypeAlias;

Type* create_type(const char* name, unsigned char bit_size, TypeOptions option);
TypeAlias* create_type_alias(const char* name, Type* refers_to);

// type_class.c
typedef struct ClassDefinitionDef {
    char* name;
    HashMap* member_variables;
    HashMap* member_functions;
    ASTNode* body;
} ClassDefinition;

ClassDefinition* create_class_definition(const char* name);

// type_function.c
typedef struct FunctionDefinitionDef {
    char* name;
    char* return_type;
    Array* parameters;
    ASTNode* body;
} FunctionDefinition;

FunctionDefinition* create_function_definition(const char* name, const char* return_type);

// type_variable.c
typedef struct VariableDef {
    char* name;
    char* type;
    bool is_array;
} Variable;

Variable* create_variable(const char* name, const char* type, bool is_array);
