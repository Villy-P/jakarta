#pragma once

#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"
#include "types/function.h"

void parse_for(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_if(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node);