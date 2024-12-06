#include <stddef.h>

#include "postfix.h"

unsigned int precedence(char* op) {
    if (op == NULL)
        return 0;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
    if (strcmp(op, "==") == 0)
        return 3;
}

Stack* infix_to_postfix(Tokenizer* tokenizer) {
    Stack* output = create_stack();
    Stack* operands = create_stack();

    int open_parenthesis_count = 1;

    while (true) {
        Token* token = consume(tokenizer);
        ASTNode* node = create_ast_node(AST_IDENTIFIER_VALUE, token);
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            add_to_stack(operands, node);
            open_parenthesis_count++;
        } else if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            open_parenthesis_count--;
            while (operands->size > 0) {
                Token* op = pop_from_stack(operands);
                if (op->symbol == SYMBOL_OPEN_PARENTHESIS)
                    break;
                add_to_stack(output, op);
            }
            if (open_parenthesis_count == 0)
                break;
        } else if (token->symbol == SYMBOL_STRING || token->symbol == SYMBOL_NUMBER) {
            add_to_stack(output, node);
        } else if (token->symbol == SYMBOL_SEMICOLON) {
            while (operands->size > 0)
                add_to_stack(output, pop_from_stack(operands));
            break;
        } else {
            while (operands->size > 0) {
                Token* op = pop_from_stack(operands);
                ASTNode* operator = create_ast_node(AST_IDENTIFIER_OPERATOR, op);
                if (precedence(op->content) < precedence(token->content)) {
                    add_to_stack(operands, operator);
                    break;
                }
                add_to_stack(output, operator);
            }
            add_to_stack(operands, node);
        }
    }
    printf("Output: ");
    print_stack(output);

    return output;
}