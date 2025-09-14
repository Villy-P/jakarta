#pragma once

#include "symbol.h"

#define INITIAL_TOKENS_LENGTH 64

// forward declerations
typedef struct TokenizerDef Tokenizer;
typedef struct ASTNodeDef ASTNode;
typedef struct FunctionDefinitionDef FunctionDefinition;

// token.c
typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);

// lexer.c
void read_line(char* line, unsigned int line_number, Tokenizer* tokenizer);
char* get_string(char** line);
char* get_string_literal(char** line);

// parser.c
void parse(Tokenizer* tokenizer, ASTNode* ast_node);

// Nodes

// AST
void parse_class(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_func(Tokenizer* tokenizer, ASTNode* ast_node);
void parse_func_call(Tokenizer* tokenizer, ASTNode* ast_node, FunctionDefinition* function);
void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node);
