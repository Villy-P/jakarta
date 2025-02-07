#include <stdlib.h>

#include "free.h"
#include "parser.h"
#include "postfix.h"
#include "data_structures/stack.h"

void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* ret = consume(tokenizer);

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* ret_node = create_ast_node(AST_IDENTIFIER_RETURN, NULL);
    add_ast_node(ret_node, expression);
    add_ast_node(ast_node, ret_node);

    free_token(ret);
}