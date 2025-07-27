#include <stdlib.h>
#include <string.h>

#include "data_structures/ast.h"
#include "types/function.h"
#include "types/types.h"
#include "parser.h"
#include "error.h"
#include "free.h"
#include "debug.h"

void parse_func(Tokenizer* tokenizer, ASTNode* ast_node) {
    add_scope(tokenizer);

    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM)
        jakarta_error_invalid_typedef_location(consume(tokenizer));

    Token* func_keyword = consume(tokenizer);
    Token* func_type = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* func_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* func_node = create_ast_node(AST_IDENTIFIER_FUNCTION_DEFINITION, func_name);

    FunctionDefinition* function_definition = create_function_definition(
        func_name->content, 
        func_type->content);

    add_function(tokenizer, function_definition);

    while (!peek(tokenizer, SYMBOL_CLOSE_PARENTHESIS)) {
        Token* arg_type = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
        Token* arg_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
        Token* comma = peek(tokenizer, SYMBOL_COMMA) ? consume(tokenizer) : NULL;
        Type* argtype = get(tokenizer->type_symbol_tree, arg_type->content);
        
        if (argtype == NULL)
            jakarta_error_undefined_identifier(arg_type);

        if (comma != NULL)
            free_token(comma);

        Parameter* parameter = create_parameter(arg_name->content, arg_type->content);
        add_to_array(function_definition->parameters, parameter);

        Variable* variable = create_variable(arg_name->content, argtype);
        add_variable_to_scope(tokenizer, variable);
    }

    Token* close_parenthesis = consume(tokenizer);
    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    Type* type = get(tokenizer->type_symbol_tree, func_type->content);
    if (type == NULL)
        jakarta_error_undefined_identifier(func_type);

    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE))
        parse(tokenizer, func_node);

    printf("WP!");

    Token* close_bracket = consume(tokenizer);
    pop_from_stack(tokenizer->variable_symbol_stack, NULL);

    add_to_array(ast_node->nodes, func_node);

    function_definition->body = malloc(sizeof(ASTNode));
    memcpy_s(function_definition->body, sizeof(ASTNode), func_node, sizeof(ASTNode));

    insert(tokenizer->function_symbol_tree, function_definition->name, function_definition);

    free_token(func_keyword);
    free_token(open_parenthesis);
    free_token(close_parenthesis);
    free_token(open_brace);
    free_token(close_bracket);
    debug_message("Added Function", TOP_LEVEL);
}

void parse_func_call(Tokenizer* tokenizer, ASTNode* ast_node, FunctionDefinition* function) {
    Token* function_name = consume(tokenizer);
    for (unsigned int i = 0; i < function->parameters->length; i++) {
        Token* arg = consume(tokenizer);
        ASTNode* arg_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETER, arg);
        add_to_array(ast_node->nodes, arg_node);
        if (i < function->parameters->length - 1)
            consume(tokenizer); // comma
    }
    free_token(function_name);
    debug_message("Parsed Function Call", TOP_LEVEL);
}