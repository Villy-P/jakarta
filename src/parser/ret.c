#include "parser.h"
#include "stack.h"

void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* ret = consume(tokenizer);

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    add_ast_node(ast_node, expression);

    ASTNode* ret_node = create_ast_node(AST_IDENTIFIER_RETURN, ret);
    add_ast_node(ast_node, ret_node);
}