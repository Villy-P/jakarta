#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "free.h"
#include "postfix.h"

void parse_if(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* if_statement = consume(tokenizer);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* if_node = create_ast_node(AST_IDENTIFIER_IF_STATEMENT, NULL);
    ASTNode* if_condition = create_ast_node(AST_IDENTIFIER_IF_CONDITION, NULL);
    ASTNode* if_body = create_ast_node(AST_IDENTIFIER_IF_BODY, NULL);

    add_ast_node(if_node, if_condition);

    printf("Parsing if statement\n");

    // parse expression here
    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);
    add_ast_node(if_condition, expression);

    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE))
        parse(tokenizer, if_body);
    Token* close_brace = consume(tokenizer);

    add_ast_node(if_node, if_body);
    add_ast_node(ast_node, if_node);

    free_token(if_statement);
    free_token(open_parenthesis);
    free_token(open_brace);
    free_token(close_brace);
}