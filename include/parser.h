#pragma once

#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"
#include "types/function.h"

void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_for(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_if(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node);
ASTNode* parse_variable_declaration(Tokenizer* tokenizer, FunctionDefinition* function_definition, ASTNode* class);