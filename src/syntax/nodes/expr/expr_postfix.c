#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/hashmap.h"
#include "data_structures/stack.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "symbol.h"
#include "syntax.h"
#include "types.h"

#define POSTFIX_STACK_INITIAL_SIZE 10

typedef enum {
    PREC_NONE = 0,
    PREC_ASSIGNMENT,    // 1
    PREC_LOGICAL_OR,    // 2
    PREC_LOGICAL_AND,   // 3
    PREC_BITWISE_OR,    // 4
    PREC_BITWISE_XOR,   // 5
    PREC_BITWISE_AND,   // 6
    PREC_EQUALITY,      // 7
    PREC_RELATIONAL,    // 8
    PREC_SHIFT,         // 9
    PREC_ADDITIVE,      // 10
    PREC_MULTIPLICATIVE,// 11
    PREC_UNARY          // 12
} Precedence;

unsigned int precedence(const char* operator) {
    if (strcmp(operator, "=") == 0 || strcmp(operator, "+=") == 0 || strcmp(operator, "-=") == 0 || strcmp(operator, "*=") == 0 || strcmp(operator, "/=") == 0 || strcmp(operator, "%=") == 0 || strcmp(operator, "&=") == 0 || strcmp(operator, "|=") == 0 || strcmp(operator, "^=") == 0 || strcmp(operator, "<<=") == 0 || strcmp(operator, ">>=") == 0 || strcmp(operator, ">>>=") == 0) {
        return PREC_ASSIGNMENT;
    } if (strcmp(operator, "||") == 0) {
        return PREC_LOGICAL_OR;
    } if (strcmp(operator, "&&") == 0) {
        return PREC_LOGICAL_AND;
    } if (strcmp(operator, "|") == 0) {
        return PREC_BITWISE_OR;
    } if (strcmp(operator, "^") == 0) {
        return PREC_BITWISE_XOR;
    } if (strcmp(operator, "&") == 0) {
        return PREC_BITWISE_AND;
    } if (strcmp(operator, "==") == 0 || strcmp(operator, "!=") == 0) {
        return PREC_EQUALITY;
    } if (strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 || strcmp(operator, "<=") == 0 || strcmp(operator, ">=") == 0) {
        return PREC_RELATIONAL;
    } if (strcmp(operator, "<<") == 0 || strcmp(operator, ">>") == 0 || strcmp(operator, ">>>") == 0) {
        return PREC_SHIFT;
    } if (strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0) {
        return PREC_ADDITIVE;
    } if (strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0 || strcmp(operator, "%") == 0) {
        return PREC_MULTIPLICATIVE;
    } if (strcmp(operator, "!") == 0 || strcmp(operator, "++") == 0 || strcmp(operator, "--") == 0 || strcmp(operator, "~") == 0) {
        return PREC_UNARY;
    }
    return 0;
}

bool is_right_associative(const char* operator) {
    // Assignment operators and exponentiation are usually right-associative
    return (strcmp(operator, "=") == 0 ||
        strcmp(operator, "+=") == 0 ||
        strcmp(operator, "-=") == 0 ||
        strcmp(operator, "*=") == 0 ||
        strcmp(operator, "/=") == 0 ||
        strcmp(operator, "%=") == 0 ||
        strcmp(operator, "^") == 0); // everything else is left-associative
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
    Stack* output = create_stack(sizeof(ASTNode), POSTFIX_STACK_INITIAL_SIZE);
    Stack* operators = create_stack(sizeof(ASTNode), POSTFIX_STACK_INITIAL_SIZE);

    int open_parenthesis_count = 1;

    while (true) {
        if (peek(tokenizer, SYMBOL_COMMA)) {
            log_msg(logs.main, "[AST] Comma found; breaking");
            while (operators->top > STACK_EMPTY) {
                ASTNode* operator = malloc(sizeof(ASTNode));
                pop_from_stack(operators, operator);
                push_to_stack(output, operator);
            }
            break;
        }
        Token* token = consume(tokenizer);
        if (!token) { 
            break;
        }

        ASTNode* node = nullptr;

        // --- String Literals ---
        if (token->symbol == SYMBOL_STRING_LITERAL) {
            log_msg(logs.main, "[AST] Processing string: %s", token->content);
            node = create_ast_node(AST_LITERAL, token);
            push_to_stack(output, node);
            continue; // skip operator logic
        }

        // --- Numbers ---
        if (token->symbol == SYMBOL_NUMBER) {
            log_msg(logs.main, "[AST] Processing number: %s", token->content);
            node = create_ast_node(AST_NUMBER, token);
            push_to_stack(output, node);
            continue; // skip operator logic
        }

        // --- Identifiers (variables or functions) ---
        if (token->symbol == SYMBOL_IDENTIFIER) {
            log_msg(logs.main, "[AST] Processing identifier: %s", token->content);

            if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
                consume(tokenizer);

                ASTNode* func_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
                do {
                    if (peek(tokenizer, SYMBOL_COMMA)) {
                        consume(tokenizer);
                    }
                    Stack* args_postfix = infix_to_postfix(tokenizer); // stops at ')'
                    const ASTNode* args_node = postfix_to_ast(args_postfix);
                    add_to_array(func_node->nodes, args_node);
                } while (peek(tokenizer, SYMBOL_COMMA));

                push_to_stack(output, func_node);
            } else {
                node = create_ast_node(AST_IDENTIFIER_VALUE, token);
                while (peek(tokenizer, SYMBOL_PERIOD)) {
                    Token* dot_token = consume(tokenizer);
                    Token* member_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
                    const ASTNode* member_node = create_ast_node(AST_IDENTIFIER_VALUE, member_token);
                    ASTNode* dot_node = create_ast_node(AST_DOT_OPERATOR, dot_token);
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
            log_msg(logs.main, "[AST] Processing array access ([]): %s", token->content);
            ASTNode* array_node = create_ast_node(AST_IDENTIFIER_ARRAY_ACCESS, nullptr);
            Stack* index_postfix = infix_to_postfix(tokenizer); // stops at ']'
            const ASTNode* index_node = postfix_to_ast(index_postfix);

            ASTNode* left_node = malloc(sizeof(ASTNode));
            pop_from_stack(output, left_node);

            add_to_array(array_node->nodes, index_node);
            ASTNode* index_wrapper = create_ast_node(AST_IDENTIFIER_INDEX, nullptr);
            add_to_array(index_wrapper->nodes, left_node);
            add_to_array(index_wrapper->nodes, array_node);

            push_to_stack(output, index_wrapper);
            continue;
        }

        // --- Parentheses ---
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            log_msg(logs.main, "[AST] Processing open parenthesis: %s", token->content);
            node = create_ast_node(AST_OPERATOR, token);
            push_to_stack(operators, node);
            open_parenthesis_count++;
            continue;
        }

        if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            log_msg(logs.main, "[AST] Processing close parenthesis: %s", token->content);
            open_parenthesis_count--;
            while (operators->top > STACK_EMPTY) {
                ASTNode* operator = malloc(sizeof(ASTNode));
                pop_from_stack(operators, operator);
                if (operator->token->symbol == SYMBOL_OPEN_PARENTHESIS) {
                    break;
                }
                push_to_stack(output, operator);
            }
            if (open_parenthesis_count == 0) {
                break;
            }
            continue;
        }

        // --- Statement endings ---
        if (token->symbol == SYMBOL_SEMICOLON || token->symbol == SYMBOL_CLOSE_BRACKET) {
            log_msg(logs.main, "[AST] Processing closing statement: %s", token->content);
            while (operators->top > STACK_EMPTY) {
                ASTNode* operator = malloc(sizeof(ASTNode));
                pop_from_stack(operators, operator);
                push_to_stack(output, operator);
            }
            break;
        }

        // --- Operators ---
        if (is_operator(token->symbol)) {
            log_msg(logs.main, "[AST] Processing operator: %s", token->content);
            node = create_ast_node(AST_OPERATOR, token);

            while (operators->top > STACK_EMPTY) {
                ASTNode* operator = malloc(sizeof(ASTNode));
                if (operator == nullptr) {
                    jakarta_error(ERR_MALLOC_FAIL, nullptr, "ASTNode");
                    return nullptr;
                }
                pop_from_stack(operators, operator);


                if (!is_operator(operator->token->symbol)) {
                    push_to_stack(operators, operator);
                    break;
                }

                if (precedence(operator->token->content) < precedence(token->content) ||
                    (precedence(operator->token->content) == precedence(token->content) && !is_right_associative(token->content))) {
                    push_to_stack(output, operator);
                } else {
                    push_to_stack(operators, operator);
                    break;
                }
            }
            push_to_stack(operators, node);
            continue;
        }

        // --- Unexpected token ---
        jakarta_error(ERR_INVALID_TOKEN, token, nullptr);
    }

    // Reverse output to maintain correct order
    reverse_stack(output);
    return output;
}

ASTNode* postfix_to_ast(Stack* postfix) {
    log_msg(logs.main, "[AST] Converting postfix to AST; stack size of %d", postfix->top + 1);
    Stack* output = create_stack(sizeof(ASTNode), POSTFIX_STACK_INITIAL_SIZE);
    ASTNode* value = malloc(sizeof(ASTNode));
    if (postfix->top == 0) {
        log_msg(logs.main, "[AST] Encountered Postfix expression with only one member variable");
        pop_from_stack(postfix, value);
        return value;
    }
    while (postfix->top > STACK_EMPTY) {
        ASTNode* node = malloc(sizeof(ASTNode));
        if (node == nullptr) {
            jakarta_error(ERR_MALLOC_FAIL, nullptr, "ASTNode");
            free(value);
            free(node);
            return nullptr;
        }
        log_msg(logs.main, "[AST] Popping from postfix stack; current size: %d", postfix->top + 1);
        pop_from_stack(postfix, node);
        if (node->token->symbol == SYMBOL_IDENTIFIER || node->token->symbol == SYMBOL_NUMBER || node->token->symbol == SYMBOL_STRING_LITERAL || node->token->symbol == SYMBOL_PERIOD) {
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
    const ASTNode* expression = postfix_to_ast(postfix);

    add_to_array(ast_node->nodes, expression);
}

void parse_variable_members(Tokenizer* tokenizer, ASTNode* ast_node, Type* type) {
    consume(tokenizer);
    Token* token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    // ClassDefinition* class = get(tokenizer->class_symbol_tree, type->name);
    printf("Parsing Variable Members\n");
    // FunctionDefinition* function = get(class->member_functions, token->content);
    if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
        // if (function == nullptr)
        //     jakarta_error(ERR_UNDEFINED_IDENTIFIER, token, "");
        const ASTNode* function_node = create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
        // parse_func_call(tokenizer, function_node, function);
        add_to_array(ast_node->nodes, function_node);
    } else if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, nullptr, token->content);
    } else {
        // Variable* variable = get(class->member_variables, token->content);
        const ASTNode* node = create_ast_node(AST_IDENTIFIER_VALUE, token);
        add_to_array(ast_node->nodes, node);
    }
    if (peek(tokenizer, SYMBOL_PERIOD)) {
        parse_variable_members(tokenizer, ast_node, type);
    }
}

TypeRegistryEntry* resolve_expression(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    switch (node->identifier) {
        case AST_IDENTIFIER_FUNCTION_CALL:
            return resolve_function_call(node, symbol_table, state);
        case AST_NUMBER:
            return (TypeRegistryEntry*)get(state->type_registry, is_decimal(node->token->content) ? "float" : "byte");
        case AST_LITERAL:
            return (TypeRegistryEntry*)get(state->type_registry, "string");
        default:
            return nullptr;
    }
}