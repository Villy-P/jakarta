#pragma once

#include "symbol.h"

#define INITIAL_TOKENS_LENGTH 64

// forward declerations
typedef struct TokenizerDef Tokenizer;
typedef struct ASTNodeDef ASTNode;
typedef struct FunctionDefinitionDef FunctionDefinition;
typedef struct TypeDef Type;
typedef struct StackDef Stack;
typedef struct CompilerStateDef CompilerState;

// token.c
typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);

// lexer.c
void read_line(char* line, char* file_name, unsigned int line_number, Tokenizer* tokenizer);
char* get_string(char** line);
char* get_string_literal(char** line);

// parser.c
void parse(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);

// Nodes

// AST
void parse_class(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_func(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_func_call(Tokenizer* tokenizer, ASTNode* ast_node, FunctionDefinition* function, CompilerState* state);
void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_import(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
ASTNode* parse_variable_declaration(Tokenizer* tokenizer, FunctionDefinition* function_definition, CompilerState* state);

// Statements
void parse_for(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_if(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);

// Expressions
Stack* infix_to_postfix(Tokenizer* tokenizer);
ASTNode* postfix_to_ast(Stack* postfix);
unsigned int precedence(char* op);
void parse_expression(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state);
void parse_variable_members(Tokenizer* tokenizer, ASTNode* ast_node, Type* type);
bool is_operator(Symbol sym);
bool is_right_associative(const char* op);
