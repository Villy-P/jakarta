#include <stdlib.h>

#include "data_structures/ast.h"
#include "data_structures/tokenizer.h"
#include "symbol.h"
#include "syntax.h"

void parse_for(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    Token* for_keyword = consume(tokenizer);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* for_node = create_ast_node(AST_IDENTIFIER_FOR, nullptr);
    ASTNode* for_initialization = create_ast_node(AST_IDENTIFIER_FOR_INITIALIZATION, nullptr);
    ASTNode* for_condition = create_ast_node(AST_IDENTIFIER_FOR_CONDITION, nullptr);
    ASTNode* for_iteration = create_ast_node(AST_IDENTIFIER_FOR_ITERATION, nullptr);
    ASTNode* for_body = create_ast_node(AST_IDENTIFIER_FOR_BODY, nullptr);

    parse_variable(tokenizer, for_node);

    ds_stack condition_postfix = infix_to_postfix(tokenizer);
    ASTNode* condition_expression = postfix_to_ast(&condition_postfix);
    ds_array_push(for_condition->nodes, condition_expression);

    ds_stack iteration_postfix = infix_to_postfix(tokenizer);
    ASTNode* iteration_expression = postfix_to_ast(&iteration_postfix);
    ds_array_push(for_iteration->nodes, iteration_expression);

    peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE)) {
        parse(tokenizer, for_body, state);
    }
    consume(tokenizer);

    ds_array_push(for_node->nodes, for_condition);
    ds_array_push(for_node->nodes, for_iteration);
    ds_array_push(for_node->nodes, for_body);

    ds_array_push(ast_node->nodes, for_node);

    free(for_keyword);
    free(open_parenthesis);
    free(for_initialization);
}