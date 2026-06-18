#include <stdlib.h>

#include "core.h"
#include "syntax.h"
#include "data_structures/stack.h"

void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    Token* ret = consume(tokenizer);

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* ret_node = create_ast_node(AST_IDENTIFIER_RETURN, NULL);
    add_to_array(ret_node->nodes, expression);
    add_to_array(ast_node->nodes, ret_node);

    free_token(ret);
}