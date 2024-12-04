#include <stddef.h>

#include "postfix.h"

unsigned int precedence(char* op) {
    if (op == NULL)
        return 0;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
}

Stack* infix_to_postfix(Tokenizer* tokenizer) {
    Stack* output = create_stack();
    Stack* operands = create_stack();

    int open_parenthesis_count = 0;

    while (!peek(tokenizer, SYMBOL_NEWLINE)) {
        Token* token = consume(tokenizer);
        if (token->symbol == SYMBOL_NUMBER || token->symbol == SYMBOL_STRING) {
            ASTNode* node = create_ast_node(AST_IDENTIFIER_VALUE, token);
            add_to_stack(output, node);
        } else if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            ASTNode* node = create_ast_node(AST_IDENTIFIER_OPERATOR, token);
            add_to_stack(operands, node);
            open_parenthesis_count++;
        } else if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            if (open_parenthesis_count == 0)
                break;
            while (operands->head != NULL && operands->head->content->token->symbol != SYMBOL_OPEN_PARENTHESIS)
                add_to_stack(output, pop_from_stack(operands));
            pop_from_stack(operands);
            open_parenthesis_count--;
        } else {
            while (operands->head != NULL && precedence(operands->head->content->token->content) >= precedence(token->content))
                add_to_stack(output, pop_from_stack(operands));
            ASTNode* node = create_ast_node(AST_IDENTIFIER_OPERATOR, token);
            add_to_stack(operands, node);
        }
    }

    return output;
}