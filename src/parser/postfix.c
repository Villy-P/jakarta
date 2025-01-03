#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "postfix.h"

unsigned int precedence(char* op) {
    if (strcmp(op, "=") == 0 || strcmp(op, "+=") == 0 || strcmp(op, "-=") == 0 || strcmp(op, "*=") == 0 || strcmp(op, "/=") == 0 || strcmp(op, "%=") == 0 || strcmp(op, "&=") == 0 || strcmp(op, "|=") == 0 || strcmp(op, "^=") == 0 || strcmp(op, "<<=") == 0 || strcmp(op, ">>=") == 0 || strcmp(op, ">>>=") == 0)
        return 1;
    if (strcmp(op, "||") == 0)
        return 2;
    if (strcmp(op, "&&") == 0)
        return 3;
    if (strcmp(op, "|") == 0)
        return 4;
    if (strcmp(op, "^") == 0)
        return 5;
    if (strcmp(op, "&") == 0)
        return 6;
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0)
        return 7;
    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0)
        return 8;
    if (strcmp(op, "<<") == 0 || strcmp(op, ">>") == 0 || strcmp(op, ">>>") == 0)
        return 9;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 10;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0)
        return 11;
    if (strcmp(op, "!") == 0 || strcmp(op, "++") == 0 || strcmp(op, "--") == 0 || strcmp(op, "~") == 0)
        return 12;
    return 0;
}

Stack* infix_to_postfix(Tokenizer* tokenizer) {
    Stack* output = create_stack(sizeof(ASTNode), 10);
    Stack* operands = create_stack(sizeof(ASTNode), 10);

    int open_parenthesis_count = 1;

    while (true) {
        Token* token = consume(tokenizer);
        ASTNode* node = create_ast_node(AST_IDENTIFIER_VALUE, token);
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            push_to_stack(operands, node);
            open_parenthesis_count++;
        } else if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            open_parenthesis_count--;
            while (operands->top > -1) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operands, op);
                if (op->token->symbol == SYMBOL_OPEN_PARENTHESIS)
                    break;
                push_to_stack(output, op);
            }
            if (open_parenthesis_count == 0)
                break;
        } else if (token->symbol == SYMBOL_STRING || token->symbol == SYMBOL_NUMBER) {
            push_to_stack(output, node);
        } else if (token->symbol == SYMBOL_SEMICOLON) {
            while (operands->top > -1) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operands, op);
                push_to_stack(output, op);
            }
            break;
        } else {
            while (operands->top > -1) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operands, op);
                if (precedence(op->token->content) < precedence(token->content)) {
                    push_to_stack(operands, op);
                    break;
                }
                push_to_stack(output, op);
            }
            push_to_stack(operands, node);
        }
    }
    printf("Output: ");
    while (output->top > -1) {
        ASTNode* node = malloc(sizeof(ASTNode));
        pop_from_stack(output, node);
        printf("%s ", node->token->content);
    }
    printf("\n");

    return output;
}