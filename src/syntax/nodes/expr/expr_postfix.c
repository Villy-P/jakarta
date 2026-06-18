#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "syntax.h"
#include "core.h"
#include "types.h"
#include "debug.h"

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

bool is_right_associative(const char* op) {
    // Assignment operators and exponentiation are usually right-associative
    if (strcmp(op, "=") == 0 ||
        strcmp(op, "+=") == 0 ||
        strcmp(op, "-=") == 0 ||
        strcmp(op, "*=") == 0 ||
        strcmp(op, "/=") == 0 ||
        strcmp(op, "%=") == 0 ||
        strcmp(op, "^") == 0) {
        return true;
    }
    return false; // everything else is left-associative
}

bool is_operator(Symbol sym) {
    return sym == SYMBOL_PLUS ||
           sym == SYMBOL_DASH ||
           sym == SYMBOL_ASTERISK ||
           sym == SYMBOL_SLASH ||
           sym == SYMBOL_PERCENT ||
           sym == SYMBOL_OPEN_ANGLE ||
           sym == SYMBOL_CLOSE_ANGLE ||
           sym == OPERATOR_ADDITION_ASSIGNMENT ||
           sym == OPERATOR_SUBTRACTION_ASSIGNMENT ||
           sym == OPERATOR_MULTIPLICATION_ASSIGNMENT ||
           sym == OPERATOR_DIVISION_ASSIGNMENT ||
           sym == OPERATOR_MODULUS_ASSIGNMENT ||
           sym == OPERATOR_BITWISE_AND_ASSIGNMENT ||
           sym == OPERATOR_BITWISE_OR_ASSIGNMENT ||
           sym == OPERATOR_BITWISE_XOR_ASSIGNMENT ||
           sym == OPERATOR_EQUIVALENCE ||
           sym == OPERATOR_NOT_EQUIVALENCE ||
           sym == OPERATOR_GREATER_THAN_OR_EQUAL_TO ||
           sym == OPERATOR_LESS_THAN_OR_EQUAL_TO ||
           sym == OPERATOR_INCREMENT ||
           sym == OPERATOR_DECREMENT ||
           sym == OPERATOR_LEFT_SHIFT ||
           sym == OPERATOR_RIGHT_SHIFT ||
           sym == OPERATOR_RIGHT_SHIFT_UNSIGNED ||
           sym == OPERATOR_NOT ||
           sym == OPERATOR_BITWISE_AND ||
           sym == OPERATOR_BITWISE_OR ||
           sym == OPERATOR_BITWISE_XOR ||
           sym == OPERATOR_BITWISE_NOT ||
           sym == OPERATOR_LOGICAL_AND ||
           sym == OPERATOR_LOGICAL_OR ||
           sym == OPERATOR_ARRAY_DECLARATION;
}

Stack* infix_to_postfix(Tokenizer* tokenizer) {
    Stack* output = create_stack(sizeof(ASTNode), 10);
    Stack* operators = create_stack(sizeof(ASTNode), 10);

    int open_parenthesis_count = 1;

    while (true) {
        Token* token = consume(tokenizer);
        if (!token) break;

        ASTNode* node = NULL;

        // --- Literals ---
        if (token->symbol == SYMBOL_NUMBER || token->symbol == SYMBOL_STRING_LITERAL) {
            fprintf(logs.main, "[POSTFIX] Processing number/string: %s\n", token->content);
            node = create_ast_node(AST_LITERAL, token);
            push_to_stack(output, node);
            continue; // skip operator logic
        }

        // --- Identifiers (variables or functions) ---
        if (token->symbol == SYMBOL_IDENTIFIER) {
            fprintf(logs.main, "[POSTFIX] Processing identifier: %s\n", token->content);

            if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
                consume(tokenizer);

                ASTNode* func_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
                Stack* args_postfix = infix_to_postfix(tokenizer); // stops at ')'
                ASTNode* args_node = postfix_to_ast(args_postfix);
                add_to_array(func_node->nodes, args_node);

                push_to_stack(output, func_node);
            } else {
                node = create_ast_node(AST_IDENTIFIER_VALUE, token);
                while (peek(tokenizer, SYMBOL_PERIOD)) {
                    consume(tokenizer);
                    Token* member_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
                    ASTNode* member_node = create_ast_node(AST_IDENTIFIER_VALUE, member_token);
                    ASTNode* dot_node = create_ast_node(AST_DOT_OPERATOR, member_token);
                    add_to_array(dot_node->nodes, node);
                    add_to_array(dot_node->nodes, member_node);
                    node = dot_node;
                }
                push_to_stack(output, node);
            }
            continue;
        }

        // --- Array access ---
        if (token->symbol == SYMBOL_OPEN_BRACKET) {
            fprintf(logs.main, "[POSTFIX] Processing array access ([]): %s\n", token->content);
            ASTNode* array_node = create_ast_node(AST_IDENTIFIER_ARRAY_ACCESS, NULL);
            Stack* index_postfix = infix_to_postfix(tokenizer); // stops at ']'
            ASTNode* index_node = postfix_to_ast(index_postfix);

            ASTNode* left_node = malloc(sizeof(ASTNode));
            pop_from_stack(output, left_node);

            add_to_array(array_node->nodes, index_node);
            ASTNode* index_wrapper = create_ast_node(AST_IDENTIFIER_INDEX, NULL);
            add_to_array(index_wrapper->nodes, left_node);
            add_to_array(index_wrapper->nodes, array_node);

            push_to_stack(output, index_wrapper);
            continue;
        }

        // --- Parentheses ---
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            fprintf(logs.main, "[POSTFIX] Processing open parenthesis: %s\n", token->content);
            node = create_ast_node(AST_OPERATOR, token);
            push_to_stack(operators, node);
            open_parenthesis_count++;
            continue;
        }

        if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            fprintf(logs.main, "[POSTFIX] Processing close parenthesis: %s\n", token->content);
            open_parenthesis_count--;
            while (operators->top > STACK_EMPTY) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operators, op);
                fprintf(logs.main, "[POSTFIX] Popped operator: %s\n", op->token->content);
                if (op->token->symbol == SYMBOL_OPEN_PARENTHESIS)
                    break;
                push_to_stack(output, op);
            }
            if (open_parenthesis_count == 0)
                break;
            continue;
        }

        // --- Statement endings ---
        if (token->symbol == SYMBOL_SEMICOLON || token->symbol == SYMBOL_CLOSE_BRACKET) {
            fprintf(logs.main, "[POSTFIX] Processing semicolon/close bracket: %s\n", token->content);
            while (operators->top > STACK_EMPTY) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operators, op);
                push_to_stack(output, op);
            }
            break;
        }

        // --- Operators ---
        if (is_operator(token->symbol)) {
            fprintf(logs.main, "[POSTFIX] Processing operator: %s\n", token->content);
            node = create_ast_node(AST_OPERATOR, token);

            while (operators->top > STACK_EMPTY) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operators, op);


                if (!is_operator(op->token->symbol)) {
                    push_to_stack(operators, op);
                    break;
                }

                if (precedence(op->token->content) < precedence(token->content) ||
                    (precedence(op->token->content) == precedence(token->content) && !is_right_associative(token->content))) {
                    push_to_stack(output, op);
                } else {
                    push_to_stack(operators, op);
                    break;
                }
            }
            push_to_stack(operators, node);
            continue;
        }

        // --- Unexpected token ---
        jakarta_error(ERR_INVALID_TOKEN, token, NULL);
    }

    // Reverse output to maintain correct order
    reverse_stack(output);
    return output;
}

ASTNode* postfix_to_ast(Stack* postfix) {
    fprintf(logs.main, "[POSTFIX_TO_AST] Converting postfix to AST\n");
    Stack* output = create_stack(sizeof(ASTNode), 10);
    ASTNode* value = malloc(sizeof(ASTNode));
    if (postfix->top == 0) {
        pop_from_stack(postfix, value);
        return value;
    }
    while (postfix->top > STACK_EMPTY) {
        ASTNode* node = malloc(sizeof(ASTNode));
        pop_from_stack(postfix, node);
        if (node->token->symbol == SYMBOL_IDENTIFIER || node->token->symbol == SYMBOL_NUMBER || node->token->symbol == SYMBOL_STRING_LITERAL) {
            push_to_stack(output, node);
        } else if (strcmp(node->token->content, "++") == 0 || strcmp(node->token->content, "--") == 0) {
            ASTNode* operand = malloc(sizeof(ASTNode));
            pop_from_stack(output, operand);
            add_to_array(node->nodes, operand);
            push_to_stack(output, node);
        } else {
            ASTNode* right = malloc(sizeof(ASTNode));
            ASTNode* left = malloc(sizeof(ASTNode));
            pop_from_stack(output, right);
            pop_from_stack(output, left); // ERROR
            add_to_array(node->nodes, left);
            add_to_array(node->nodes, right);
            push_to_stack(output, node);
        }
    }
    pop_from_stack(output, value);
    return value;
}

void parse_expression(Tokenizer* tokenizer, ASTNode* ast_node) {
    // first, check if its a variable declaration. these can be in four forms:
    // 1. TYPE IDENTIFIER;
    // 2. TYPE IDENTIFIER = EXPRESSION;
    // 3. TYPE[] IDENTIFIER; (array declaration)
    // 4. TYPE[] IDENTIFIER = EXPRESSION; (array declaration with initialization)
    if ((peek(tokenizer, SYMBOL_IDENTIFIER) && peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 1) && peek_ahead(tokenizer, SYMBOL_SEMICOLON, 2)) ||
        (peek(tokenizer, SYMBOL_IDENTIFIER) && peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 1) && peek_ahead(tokenizer, SYMBOL_EQUALS, 2)) ||
        (peek(tokenizer, SYMBOL_IDENTIFIER) && peek_ahead(tokenizer, OPERATOR_ARRAY_DECLARATION, 1) && peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 2)) ||
        (peek(tokenizer, SYMBOL_IDENTIFIER) && peek_ahead(tokenizer, OPERATOR_ARRAY_DECLARATION, 1) && peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 2) && peek_ahead(tokenizer, SYMBOL_EQUALS, 3))) {
        parse_variable(tokenizer, ast_node);
        return;
    }

    Stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    add_to_array(ast_node->nodes, expression);
}

void parse_variable_members(Tokenizer* tokenizer, ASTNode* ast_node, Type* type) {
    consume(tokenizer);
    Token* token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    // ClassDefinition* class = get(tokenizer->class_symbol_tree, type->name);
    printf("Parsing Variable Members\n");
    // FunctionDefinition* function = get(class->member_functions, token->content);
    if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
        // if (function == NULL)
        //     jakarta_error(ERR_UNDEFINED_IDENTIFIER, token, "");
        ASTNode* function_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
        // parse_func_call(tokenizer, function_node, function);
        add_to_array(ast_node->nodes, function_node);
    } else if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, NULL, token->content);
    } else {
        // Variable* variable = get(class->member_variables, token->content);
        ASTNode* node = create_ast_node(AST_IDENTIFIER_VALUE, token);
        add_to_array(ast_node->nodes, node);
    }
    if (peek(tokenizer, SYMBOL_PERIOD))
        parse_variable_members(tokenizer, ast_node, type);
}