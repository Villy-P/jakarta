#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/stack.h"
#include "data_structures/tokenizer.h"
#include "syntax.h"


void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* ret = consume(tokenizer);

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* ret_node = create_ast_node(AST_IDENTIFIER_RETURN, nullptr);
    ds_array_push(ret_node->nodes, expression);
    ds_array_push(ast_node->nodes, ret_node);

    free_token(ret);
}