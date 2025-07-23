#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "postfix.h"
#include "parser.h"
#include "error.h"

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

bool is_right_associative(char* op) {
    if (strcmp(op, "^") == 0 || strcmp(op, "&&") == 0 || strcmp(op, "||") == 0)
        return true;
    return false;
}

ASTNode* parse_expression(Tokenizer* tokenizer, bool stop_on_comma) {
    Stack* output = create_stack(sizeof(ASTNode*), 10);
    Stack* operands = create_stack(sizeof(ASTNode*), 10);

    Token* token = consume(tokenizer);

    while (token != NULL && !peek(tokenizer, SYMBOL_SEMICOLON)) {
        if (stop_on_comma && (token->symbol == SYMBOL_COMMA || token->symbol == SYMBOL_CLOSE_PARENTHESIS))
            break;
        
        if (token->symbol == SYMBOL_NUMBER) {
            ASTNode* number_node = create_ast_node(AST_IDENTIFIER_VALUE, token);
            push_to_stack(output, number_node);
        } else if (token->symbol == SYMBOL_STRING) {
            bool next_token = peek(tokenizer, SYMBOL_OPEN_PARENTHESIS);
            if (next_token != false) {
                FunctionDefinition* function = get(tokenizer->function_symbol_tree, token->content);
                if (function != NULL) {
                    ASTNode* function_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
                    parse_func_call(tokenizer, function_node, function);
                    push_to_stack(output, function_node);
                } else {
                    jakarta_error_undefined_identifier(token);
                    return NULL;
                }
            } else {
                ASTNode* variable_node = create_ast_node(AST_IDENTIFIER_VARIABLE_CONTENT, token);
                push_to_stack(output, variable_node);
            }
        } else if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            ASTNode* open_parenthesis_node = create_ast_node(AST_IDENTIFIER_OPERATOR, token);
            push_to_stack(operands, open_parenthesis_node);
        } else if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            while (operands->top > -1) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operands, op);
                if (op->token->symbol == SYMBOL_OPEN_PARENTHESIS)
                    break;
                push_to_stack(output, op);
            }
        } else if (precedence(token->content) != 0) {
            while (operands->top > -1) {
                ASTNode* op = malloc(sizeof(ASTNode));
                pop_from_stack(operands, op);
                if (precedence(op->token->content) < precedence(token->content) ||
                    (precedence(op->token->content) == precedence(token->content) && is_right_associative(token->content))) {
                    push_to_stack(operands, op);
                    break;
                }
                push_to_stack(output, op);
            }
            ASTNode* operator_node = create_ast_node(AST_IDENTIFIER_OPERATOR, token);
            push_to_stack(operands, operator_node);
        }
    }

    while (operands->top > -1) {
        ASTNode* op = malloc(sizeof(ASTNode));
        ASTNode* right = malloc(sizeof(ASTNode));
        ASTNode* left = malloc(sizeof(ASTNode));
        pop_from_stack(operands, op);
        pop_from_stack(output, right);
        pop_from_stack(output, left);
        add_to_array(op->nodes, left);
        add_to_array(op->nodes, right);
        push_to_stack(output, op);
    }

    ASTNode* root = malloc(sizeof(ASTNode));
    pop_from_stack(output, root);

    return root;
}

ASTNode* parse_func_call(Tokenizer* tokenizer, ASTNode* function_node, FunctionDefinition* function) {
    peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);
    Array* parameters = create_array(sizeof(ASTNode*));
    while (peek(tokenizer, SYMBOL_CLOSE_PARENTHESIS) == false) {
        ASTNode* arg_node = parse_expression(tokenizer, true);
        add_to_array(parameters, arg_node);
        if (peek(tokenizer, SYMBOL_COMMA))
            consume(tokenizer); 
    }

    ASTNode* function_call_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, function_node->token);
    function_call_node->nodes = parameters;

    peek_consume(tokenizer, SYMBOL_CLOSE_PARENTHESIS);
    // TODO: Check if parameters match function definition
    return function_call_node;
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
            FunctionDefinition* function = get(tokenizer->function_symbol_tree, token->content);
            if (function != NULL && peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
                ASTNode* function_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
                parse_func_call(tokenizer, function_node, function);
                push_to_stack(output, function_node);
            } else {
                push_to_stack(output, node);
            }
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

    reverse_stack(output);

    return output;
}

ASTNode* postfix_to_ast(Stack* postfix) {
    printf("Converting postfix to AST\n");
    Stack* output = create_stack(sizeof(ASTNode), 10);
    ASTNode* value = malloc(sizeof(ASTNode));
    if (postfix->top == 0) {
        pop_from_stack(postfix, value);
        return value;
    }
    while (postfix->top > -1) {
        ASTNode* node = malloc(sizeof(ASTNode));
        pop_from_stack(postfix, node);
        if (node->token->symbol == SYMBOL_STRING || node->token->symbol == SYMBOL_NUMBER) {
            push_to_stack(output, node);
        } else {
            ASTNode* right = malloc(sizeof(ASTNode));
            ASTNode* left = malloc(sizeof(ASTNode));
            pop_from_stack(output, right);
            pop_from_stack(output, left);
            add_to_array(node->nodes, left);
            add_to_array(node->nodes, right);
            push_to_stack(output, node);
        }
    }
    pop_from_stack(output, value);
    return value;
}