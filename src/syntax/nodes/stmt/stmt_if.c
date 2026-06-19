#include <stdlib.h>

#include "data_structures/ast.h"
#include "syntax.h"
#include "core.h"

void parse_if(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {    
    Token* if_statement = consume(tokenizer);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* if_node = create_ast_node(AST_IDENTIFIER_IF_STATEMENT, NULL);
    ASTNode* if_condition = create_ast_node(AST_IDENTIFIER_IF_CONDITION, NULL);
    ASTNode* if_body = create_ast_node(AST_IDENTIFIER_IF_BODY, NULL);

    // parse expression here
    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);
    add_to_array(if_condition->nodes, expression);

    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE))
        parse(tokenizer, if_body, state);
    Token* close_brace = consume(tokenizer);

    add_to_array(if_node->nodes, if_body);
    add_to_array(ast_node->nodes, if_node);

    free_token(if_statement);
    free_token(open_parenthesis);
    free_token(open_brace);
    free_token(close_brace);
}