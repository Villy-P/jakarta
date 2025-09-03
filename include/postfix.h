#pragma once

#include "data_structures/tokenizer.h"
#include "data_structures/stack.h"

Stack* infix_to_postfix(Tokenizer* tokenizer);
ASTNode* postfix_to_ast(Stack* postfix);
unsigned int precedence(char* op);
void parse_expression(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_variable_members(Tokenizer* tokenizer, ASTNode* ast_node, Type* type);
bool is_operator(Symbol sym);
bool is_right_associative(const char* op);