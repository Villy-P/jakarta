#pragma once

#include "data_structures/compiler_state.h"

void gather_declarations(CompilerState* state);
void resolve_types(CompilerState* state);

void resolve_node(ASTNode* node, SymbolTable* symbol_table, CompilerState* state);