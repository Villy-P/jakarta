#ifndef __JAKARTA_POSTFIX_H__
#define __JAKARTA_POSTFIX_H__

#include "data_structures/tokenizer.h"
#include "data_structures/stack.h"

ASTNode* parse_expression(Tokenizer* tokenizer, bool stop_on_comma);
ASTNode* parse_func_call(Tokenizer* tokenizer, ASTNode* function_node, FunctionDefinition* function);
Stack* infix_to_postfix(Tokenizer* tokenizer);
ASTNode* postfix_to_ast(Stack* postfix);
unsigned int precedence(char* op);

#endif