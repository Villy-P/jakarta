#ifndef __JAKARTA_POSTFIX_H__
#define __JAKARTA_POSTFIX_H__

#include "data_structures/tokenizer.h"
#include "data_structures/stack.h"

Stack* infix_to_postfix(Tokenizer* tokenizer);
ASTNode* postfix_to_ast(Stack* postfix);
unsigned int precedence(char* op);
void parse_expression(Tokenizer* tokenizer, ASTNode* ast_node);

#endif