#ifndef __JAKARTA_PARSER_H__
#define __JAKARTA_PARSER_H__

#include "tokenizer.h"
#include "ast.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_func(Tokenizer* tokenizer, ASTNode* ast_node);

#endif