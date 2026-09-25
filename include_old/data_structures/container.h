#pragma once

#include "ds_array.h"
#include "ds_stack.h"

typedef struct ASTNodeDef ASTNode;
typedef struct ForestEntryDef ForestEntry;
typedef struct SymbolTableDef SymbolTable;
typedef struct TokenDef Token;

DS_DEFINE_ARRAY(int, ds_int_array, NULL, NULL)
DS_DEFINE_ARRAY(ASTNode*, ds_astnode_ptr_array, NULL, NULL)
DS_DEFINE_ARRAY(char*, ds_char_ptr_array, NULL, NULL)
DS_DEFINE_ARRAY(ForestEntry*, ds_forest_entry_ptr_array, NULL, NULL)
DS_DEFINE_ARRAY(Token*, ds_token_ptr_array, NULL, NULL)
DS_DEFINE_ARRAY(SymbolTable*, ds_symbol_table_ptr_array, NULL, NULL)

DS_DEFINE_STACK(ASTNode*, ds_astnode_ptr_stack, NULL, NULL)