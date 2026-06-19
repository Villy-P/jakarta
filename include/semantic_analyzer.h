#pragma once

#include "data_structures/compiler_state.h"
#include "data_structures/symbol_table.h"

void gather_declarations(CompilerState* state);
void resolve_types(CompilerState* state);

void resolve_node(ASTNode* node, SymbolTable* symbol_table, CompilerState* state);
SymbolTableEntry* lookup_type(const char* target, SymbolTable* symbol_table, CompilerState* state);

void resolve_function_definition(ASTNode* node, SymbolTable* symbol_table, CompilerState* state);
void resolve_variable_definition(ASTNode* node, SymbolTable* symbol_table, CompilerState* state);
void resolve_expression(ASTNode* node, SymbolTable* symbol_table, CompilerState* state);
void resolve_function_call(ASTNode* node, SymbolTable* symbol_table, CompilerState* state);