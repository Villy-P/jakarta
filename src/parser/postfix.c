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

    int open_parenthesis_count = 0;

    while (true) {
        Token* token = consume(tokenizer);
        printf("Token: %s\n", token->content);
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS)
            open_parenthesis_count++;
        else if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            open_parenthesis_count--;
            if (open_parenthesis_count < 0)
                break;
        }
    }

    return output;
}