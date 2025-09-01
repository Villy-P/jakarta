#ifndef __JAKARTA_PARSER_H__
#define __JAKARTA_PARSER_H__

#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"
#include "types/function.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_for(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_func(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_func_call(Tokenizer* tokenizer, ASTNode* ast_node, FunctionDefinition* function);
void parse_if(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_class(Tokenizer* tokenizer, ASTNode* ast_node);
ASTNode* parse_variable_declaration(Tokenizer* tokenizer, FunctionDefinition* function_definition, ASTNode* class);

#endif