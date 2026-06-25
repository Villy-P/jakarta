#include "core.h"
#include "data_structures/array.h"
#include "data_structures/ast.h"
#include "data_structures/stack.h"
#include "data_structures/tokenizer.h"
#include "syntax.h"


void parse_ret(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* ret = consume(tokenizer);

    Stack* postfix = infix_to_postfix(tokenizer);
    const ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* ret_node = create_ast_node(AST_IDENTIFIER_RETURN, nullptr);
    add_to_array(ret_node->nodes, expression);
    add_to_array(ast_node->nodes, ret_node);

    free_token(ret);
}