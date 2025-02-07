#ifndef __JAKARTA_PARSER_H__
#define __JAKARTA_PARSER_H__

#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_func(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_if(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node);

#endif