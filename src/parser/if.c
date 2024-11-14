#include <stdlib.h>

#include "parser.h"
#include "free.h"

void parse_if(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* if_statement = consume(tokenizer);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* if_node = create_ast_node(AST_IDENTIFIER_IF_STATEMENT, NULL);
    ASTNode* if_condition = create_ast_node(AST_IDENTIFIER_IF_CONDITION, NULL);
    ASTNode* if_body = create_ast_node(AST_IDENTIFIER_IF_BODY, NULL);

    add_ast_node(if_node, if_condition);

    // parse expression here
    int parenthesis_count = 1;
    while (parenthesis_count > 0) {
        Token* token = consume(tokenizer);
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            parenthesis_count++;
        } else if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            parenthesis_count--;
        }
        free_token(token);
    }

    // Token* close_parenthesis = peek_consume(tokenizer, SYMBOL_CLOSE_PARENTHESIS);
    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE))
        parse(tokenizer, ast_node);
    Token* close_brace = consume(tokenizer);

    add_ast_node(if_node, if_body);
    add_ast_node(ast_node, if_node);

    free_token(if_statement);
    free_token(open_parenthesis);
    // free_token(close_parenthesis);
    free_token(open_brace);
    free_token(close_brace);
}