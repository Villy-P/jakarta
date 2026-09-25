#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/container.h"
#include "data_structures/tokenizer.h"
#include "symbol.h"
#include "syntax.h"

void parse_if(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    Token* if_statement = consume(tokenizer);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* if_node = create_ast_node(AST_IDENTIFIER_IF_STATEMENT, nullptr);
    ASTNode* if_condition =
        create_ast_node(AST_IDENTIFIER_IF_CONDITION, nullptr);
    ASTNode* if_body = create_ast_node(AST_IDENTIFIER_IF_BODY, nullptr);

    // parse expression here
    ds_astnode_ptr_stack postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(&postfix);
    ds_astnode_ptr_array_push(if_condition->nodes, expression);

    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE)) {
        parse(tokenizer, if_body, state);
    }
    Token* close_brace = consume(tokenizer);

    ds_astnode_ptr_array_push(if_node->nodes, if_condition);
    ds_astnode_ptr_array_push(if_node->nodes, if_body);
    ds_astnode_ptr_array_push(ast_node->nodes, if_node);

    free_token(if_statement);
    free_token(open_parenthesis);
    free_token(open_brace);
    free_token(close_brace);
}