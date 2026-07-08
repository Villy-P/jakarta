#define DS_C_IMPLEMENTATION

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/container.h"
#include "data_structures/hashmap.h"
#include "data_structures/tokenizer.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "symbol.h"
#include "syntax.h"

static const size_t POSTFIX_STACK_INITIAL_SIZE = 10;

typedef enum {
    PREC_NONE = 0,
    PREC_ASSIGNMENT,      // 1
    PREC_LOGICAL_OR,      // 2
    PREC_LOGICAL_AND,     // 3
    PREC_BITWISE_OR,      // 4
    PREC_BITWISE_XOR,     // 5
    PREC_BITWISE_AND,     // 6
    PREC_EQUALITY,        // 7
    PREC_RELATIONAL,      // 8
    PREC_SHIFT,           // 9
    PREC_ADDITIVE,        // 10
    PREC_MULTIPLICATIVE,  // 11
    PREC_UNARY            // 12
} Precedence;

unsigned int precedence(const char* operator) {
    if (strcmp(operator, "=") == 0 || strcmp(operator, "+=") == 0 ||
        strcmp(operator, "-=") == 0 || strcmp(operator, "*=") == 0 ||
        strcmp(operator, "/=") == 0 || strcmp(operator, "%=") == 0 ||
        strcmp(operator, "&=") == 0 || strcmp(operator, "|=") == 0 ||
        strcmp(operator, "^=") == 0 || strcmp(operator, "<<=") == 0 ||
        strcmp(operator, ">>=") == 0 || strcmp(operator, ">>>=") == 0) {
        return PREC_ASSIGNMENT;
    }
    if (strcmp(operator, "||") == 0) {
        return PREC_LOGICAL_OR;
    }
    if (strcmp(operator, "&&") == 0) {
        return PREC_LOGICAL_AND;
    }
    if (strcmp(operator, "|") == 0) {
        return PREC_BITWISE_OR;
    }
    if (strcmp(operator, "^") == 0) {
        return PREC_BITWISE_XOR;
    }
    if (strcmp(operator, "&") == 0) {
        return PREC_BITWISE_AND;
    }
    if (strcmp(operator, "==") == 0 || strcmp(operator, "!=") == 0) {
        return PREC_EQUALITY;
    }
    if (strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
        strcmp(operator, "<=") == 0 || strcmp(operator, ">=") == 0) {
        return PREC_RELATIONAL;
    }
    if (strcmp(operator, "<<") == 0 || strcmp(operator, ">>") == 0 ||
        strcmp(operator, ">>>") == 0) {
        return PREC_SHIFT;
    }
    if (strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0) {
        return PREC_ADDITIVE;
    }
    if (strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0 ||
        strcmp(operator, "%") == 0) {
        return PREC_MULTIPLICATIVE;
    }
    if (strcmp(operator, "!") == 0 || strcmp(operator, "++") == 0 ||
        strcmp(operator, "--") == 0 || strcmp(operator, "~") == 0) {
        return PREC_UNARY;
    }
    return 0;
}

bool is_right_associative(const char* operator) {
    // Assignment operators and exponentiation are usually right-associative
    return (strcmp(operator, "=") == 0 || strcmp(operator, "+=") == 0 ||
            strcmp(operator, "-=") == 0 || strcmp(operator, "*=") == 0 ||
            strcmp(operator, "/=") == 0 || strcmp(operator, "%=") == 0 ||
            strcmp(operator, "^") == 0) !=
           0;  // everything else is left-associative
}

bool is_operator(Symbol sym) {
    return (sym == SYMBOL_PLUS || sym == SYMBOL_DASH ||
            sym == SYMBOL_ASTERISK || sym == SYMBOL_SLASH ||
            sym == SYMBOL_PERCENT || sym == SYMBOL_OPEN_ANGLE ||
            sym == SYMBOL_CLOSE_ANGLE || sym == OPERATOR_ADDITION_ASSIGNMENT ||
            sym == OPERATOR_SUBTRACTION_ASSIGNMENT ||
            sym == OPERATOR_MULTIPLICATION_ASSIGNMENT ||
            sym == OPERATOR_DIVISION_ASSIGNMENT ||
            sym == OPERATOR_MODULUS_ASSIGNMENT ||
            sym == OPERATOR_BITWISE_AND_ASSIGNMENT ||
            sym == OPERATOR_BITWISE_OR_ASSIGNMENT ||
            sym == OPERATOR_BITWISE_XOR_ASSIGNMENT ||
            sym == OPERATOR_EQUIVALENCE || sym == OPERATOR_NOT_EQUIVALENCE ||
            sym == OPERATOR_GREATER_THAN_OR_EQUAL_TO ||
            sym == OPERATOR_LESS_THAN_OR_EQUAL_TO ||
            sym == OPERATOR_INCREMENT || sym == OPERATOR_DECREMENT ||
            sym == OPERATOR_LEFT_SHIFT || sym == OPERATOR_RIGHT_SHIFT ||
            sym == OPERATOR_RIGHT_SHIFT_UNSIGNED || sym == OPERATOR_NOT ||
            sym == OPERATOR_BITWISE_AND || sym == OPERATOR_BITWISE_OR ||
            sym == OPERATOR_BITWISE_XOR || sym == OPERATOR_BITWISE_NOT ||
            sym == OPERATOR_LOGICAL_AND || sym == OPERATOR_LOGICAL_OR ||
            sym == OPERATOR_ARRAY_DECLARATION) != 0;
}

ds_astnode_ptr_stack infix_to_postfix(Tokenizer* tokenizer) {
    ds_astnode_ptr_stack output = {};
    ds_astnode_ptr_stack operators = {};
    ds_astnode_ptr_stack_init(&output, POSTFIX_STACK_INITIAL_SIZE);
    ds_astnode_ptr_stack_init(&operators, POSTFIX_STACK_INITIAL_SIZE);

    int open_parenthesis_count = 1;

    while (true) {
        if (peek(tokenizer, SYMBOL_COMMA)) {
            log_msg(logs.main, "[AST] Comma found; breaking");
            while (operators.length > 0) {
                ASTNode* operator= nullptr;
                ds_astnode_ptr_stack_pop(&operators, &operator);
                ds_astnode_ptr_stack_push(&output, operator);
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
            ds_astnode_ptr_stack_push(&output, node);
            continue;  // skip operator logic
        }

        // --- Numbers ---
        if (token->symbol == SYMBOL_NUMBER) {
            log_msg(logs.main, "[AST] Processing number: %s", token->content);
            node = create_ast_node(AST_NUMBER, token);
            ds_astnode_ptr_stack_push(&output, node);
            continue;  // skip operator logic
        }

        // --- Identifiers (variables or functions) ---
        if (token->symbol == SYMBOL_IDENTIFIER) {
            log_msg(logs.main, "[AST] Processing identifier: %s",
                    token->content);

            if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
                consume(tokenizer);

                ASTNode* func_node =
                    create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
                do {
                    if (peek(tokenizer, SYMBOL_COMMA)) {
                        consume(tokenizer);
                    }
                    ds_astnode_ptr_stack args_postfix =
                        infix_to_postfix(tokenizer);  // stops at ')'
                    ASTNode* args_node = postfix_to_ast(&args_postfix);
                    ds_astnode_ptr_array_push(func_node->nodes, args_node);
                } while (peek(tokenizer, SYMBOL_COMMA));

                ds_astnode_ptr_stack_push(&output, func_node);
            } else {
                node = create_ast_node(AST_IDENTIFIER_VALUE, token);
                while (peek(tokenizer, SYMBOL_PERIOD)) {
                    Token* dot_token = consume(tokenizer);
                    Token* member_token =
                        peek_consume(tokenizer, SYMBOL_IDENTIFIER);
                    ASTNode* member_node =
                        create_ast_node(AST_IDENTIFIER_VALUE, member_token);
                    ASTNode* dot_node =
                        create_ast_node(AST_DOT_OPERATOR, dot_token);
                    ds_astnode_ptr_array_push(dot_node->nodes, node);
                    ds_astnode_ptr_array_push(dot_node->nodes, member_node);
                    node = dot_node;
                }
                ds_astnode_ptr_stack_push(&output, node);
            }
            continue;
        }

        // --- Array access ---
        if (token->symbol == SYMBOL_OPEN_BRACKET) {
            log_msg(logs.main, "[AST] Processing array access ([]): %s",
                    token->content);
            ASTNode* array_node =
                create_ast_node(AST_IDENTIFIER_ARRAY_ACCESS, nullptr);
            ds_astnode_ptr_stack index_postfix =
                infix_to_postfix(tokenizer);  // stops at ']'
            ASTNode* index_node = postfix_to_ast(&index_postfix);

            ASTNode* left_node = nullptr;
            ds_astnode_ptr_stack_pop(&output, &left_node);

            ds_astnode_ptr_array_push(array_node->nodes, index_node);
            ASTNode* index_wrapper =
                create_ast_node(AST_IDENTIFIER_INDEX, nullptr);
            ds_astnode_ptr_array_push(index_wrapper->nodes, left_node);
            ds_astnode_ptr_array_push(index_wrapper->nodes, array_node);

            ds_astnode_ptr_stack_push(&output, index_wrapper);
            continue;
        }

        // --- Parentheses ---
        if (token->symbol == SYMBOL_OPEN_PARENTHESIS) {
            log_msg(logs.main, "[AST] Processing open parenthesis: %s",
                    token->content);
            node = create_ast_node(AST_OPERATOR, token);
            ds_astnode_ptr_stack_push(&operators, node);
            open_parenthesis_count++;
            continue;
        }

        if (token->symbol == SYMBOL_CLOSE_PARENTHESIS) {
            log_msg(logs.main, "[AST] Processing close parenthesis: %s",
                    token->content);
            open_parenthesis_count--;
            while (operators.length > 0) {
                ASTNode* operator= nullptr;
                ds_astnode_ptr_stack_pop(&operators, &operator);
                if (operator->token->symbol == SYMBOL_OPEN_PARENTHESIS) {
                    break;
                }
                ds_astnode_ptr_stack_push(&output, operator);
            }
            if (open_parenthesis_count == 0) {
                break;
            }
            continue;
        }

        // --- Statement endings ---
        if (token->symbol == SYMBOL_SEMICOLON ||
            token->symbol == SYMBOL_CLOSE_BRACKET) {
            log_msg(logs.main, "[AST] Processing closing statement: %s",
                    token->content);
            while (operators.length > 0) {
                ASTNode* operator= nullptr;
                ds_astnode_ptr_stack_pop(&operators, &operator);
                ds_astnode_ptr_stack_push(&output, operator);
            }
            break;
        }

        // --- Operators ---
        if (is_operator(token->symbol)) {
            log_msg(logs.main, "[AST] Processing operator: %s", token->content);
            node = create_ast_node(AST_OPERATOR, token);

            while (operators.length > 0) {
                ASTNode* operator= nullptr;
                ds_astnode_ptr_stack_pop(&operators, &operator);

                if (!is_operator(operator->token->symbol)) {
                    ds_astnode_ptr_stack_push(&operators, operator);
                    break;
                }

                if (precedence(operator->token->content) <
                        precedence(token->content) ||
                    (precedence(operator->token->content) ==
                         precedence(token->content) &&
                     !is_right_associative(token->content))) {
                    ds_astnode_ptr_stack_push(&output, operator);
                } else {
                    ds_astnode_ptr_stack_push(&operators, operator);
                    break;
                }
            }
            ds_astnode_ptr_stack_push(&operators, node);
            continue;
        }

        // --- Unexpected token ---
        jakarta_error(ERR_INVALID_TOKEN, token, nullptr);
    }

    // Reverse output to maintain correct order
    ds_astnode_ptr_stack_reverse(&output);
    return output;
}

ASTNode* postfix_to_ast(ds_astnode_ptr_stack* postfix) {
    log_msg(logs.main, "[AST] Converting postfix to AST; stack size of %d",
            postfix->length + 1);
    ds_astnode_ptr_stack* output =
        ds_astnode_ptr_stack_create(POSTFIX_STACK_INITIAL_SIZE);
    if (postfix->length <= 1) {
        log_msg(logs.main,
                "[AST] Encountered Postfix expression with only one member "
                "variable");
        ASTNode* node = nullptr;
        ds_astnode_ptr_stack_pop(postfix, &node);
        return node;
    }
    while (postfix->length > 0) {
        ASTNode* node = nullptr;
        ds_astnode_ptr_stack_pop(postfix, &node);
        log_msg(logs.main, "[AST] Popping from postfix stack; current size: %d",
                postfix->length + 1);
        if (node->token->symbol == SYMBOL_IDENTIFIER ||
            node->token->symbol == SYMBOL_NUMBER ||
            node->token->symbol == SYMBOL_STRING_LITERAL ||
            node->token->symbol == SYMBOL_PERIOD) {
            ds_astnode_ptr_stack_push(output, node);
        } else if (strcmp(node->token->content, "++") == 0 ||
                   strcmp(node->token->content, "--") == 0) {
            ASTNode* operand = nullptr;
            ds_astnode_ptr_stack_pop(output, &operand);
            ds_astnode_ptr_array_push(node->nodes, operand);
            ds_astnode_ptr_stack_push(output, node);
        } else {
            ASTNode* right = nullptr;
            ASTNode* left = nullptr;
            ds_astnode_ptr_stack_pop(output, &right);
            ds_astnode_ptr_stack_pop(output, &left);
            ds_astnode_ptr_array_push(node->nodes, left);
            ds_astnode_ptr_array_push(node->nodes, right);
            ds_astnode_ptr_stack_push(output, node);
        }
    }
    ASTNode* result = nullptr;
    ds_astnode_ptr_stack_pop(output, &result);
    return result;
}

void parse_expression(Tokenizer* tokenizer, ASTNode* ast_node) {
    // first, check if its a variable declaration. these can be in four forms:
    // 1. TYPE IDENTIFIER;
    // 2. TYPE IDENTIFIER = EXPRESSION;
    // 3. TYPE[] IDENTIFIER; (array declaration)
    // 4. TYPE[] IDENTIFIER = EXPRESSION; (array declaration with
    // initialization)
    if ((peek(tokenizer, SYMBOL_IDENTIFIER) &&
         peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 1) &&
         peek_ahead(tokenizer, SYMBOL_SEMICOLON, 2)) ||
        (peek(tokenizer, SYMBOL_IDENTIFIER) &&
         peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 1) &&
         peek_ahead(tokenizer, SYMBOL_EQUALS, 2)) ||
        (peek(tokenizer, SYMBOL_IDENTIFIER) &&
         peek_ahead(tokenizer, OPERATOR_ARRAY_DECLARATION, 1) &&
         peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 2)) ||
        (peek(tokenizer, SYMBOL_IDENTIFIER) &&
         peek_ahead(tokenizer, OPERATOR_ARRAY_DECLARATION, 1) &&
         peek_ahead(tokenizer, SYMBOL_IDENTIFIER, 2) &&
         peek_ahead(tokenizer, SYMBOL_EQUALS, 3))) {
        parse_variable(tokenizer, ast_node);
        return;
    }

    ds_astnode_ptr_stack postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(&postfix);

    ds_astnode_ptr_array_push(ast_node->nodes, expression);
}

void parse_variable_members(Tokenizer* tokenizer, ASTNode* ast_node,
                            Type* type) {
    consume(tokenizer);
    Token* token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    // ClassDefinition* class = get(tokenizer->class_symbol_tree, type->name);
    printf("Parsing Variable Members\n");
    // FunctionDefinition* function = get(class->member_functions,
    // token->content);
    if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
        // if (function == nullptr)
        //     jakarta_error(ERR_UNDEFINED_IDENTIFIER, token, "");
        ASTNode* function_node =
            create_ast_node(AST_IDENTIFIER_FUNCTION_CALL, token);
        // parse_func_call(tokenizer, function_node, function);
        ds_astnode_ptr_array_push(ast_node->nodes, function_node);
    } else if (peek(tokenizer, SYMBOL_OPEN_PARENTHESIS)) {
        jakarta_error(ERR_UNDEFINED_IDENTIFIER, nullptr, token->content);
    } else {
        // Variable* variable = get(class->member_variables, token->content);
        ASTNode* node = create_ast_node(AST_IDENTIFIER_VALUE, token);
        ds_astnode_ptr_array_push(ast_node->nodes, node);
    }
    if (peek(tokenizer, SYMBOL_PERIOD)) {
        parse_variable_members(tokenizer, ast_node, type);
    }
}

TypeRegistryEntry* resolve_expression(ASTNode* node, SymbolTable* symbol_table,
                                      CompilerState* state) {
    switch (node->identifier) {
        case AST_IDENTIFIER_FUNCTION_CALL:
            return resolve_function_call(node, symbol_table, state);
        case AST_NUMBER:
            return (TypeRegistryEntry*)get(
                state->type_registry,
                (int)is_decimal(node->token->content) ? "float" : "byte");
        case AST_LITERAL:
            return (TypeRegistryEntry*)get(state->type_registry, "string");
        default:
            return nullptr;
    }
}