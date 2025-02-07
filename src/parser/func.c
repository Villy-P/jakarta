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
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM)
        jakarta_error_invalid_typedef_location(consume(tokenizer));

    Token* func_keyword = consume(tokenizer);
    Token* func_type = peek_consume(tokenizer, SYMBOL_STRING);
    Token* func_name = peek_consume(tokenizer, SYMBOL_STRING);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* func_node = create_ast_node(AST_IDENTIFIER_FUNCTION_DEFINITION, func_name);

    FunctionDefinition* function_definition = create_function_definition(
        func_name->content, 
        func_type->content);

    while (!peek(tokenizer, SYMBOL_CLOSE_PARENTHESIS)) {
        Token* arg_type = peek_consume(tokenizer, SYMBOL_STRING);
        Token* arg_name = peek_consume(tokenizer, SYMBOL_STRING);
        Token* comma = peek(tokenizer, SYMBOL_COMMA) ? consume(tokenizer) : NULL;
        Type* argtype = get(tokenizer->type_symbol_tree, arg_type->content);
        
        if (argtype == NULL)
            jakarta_error_undefined_identifier(arg_type);

        if (comma != NULL)
            free_token(comma);

        Parameter* parameter = create_parameter(arg_name->content, arg_type->content);
        add_parameter(function_definition, parameter);
    }

    Token* close_parenthesis = consume(tokenizer);
    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    Type* type = get(tokenizer->type_symbol_tree, func_type->content);
    if (type == NULL)
        jakarta_error_undefined_identifier(func_type);

    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE))
        parse(tokenizer, func_node);

    Token* close_bracket = consume(tokenizer);

    add_ast_node(ast_node, func_node);

    function_definition->body = malloc(sizeof(ASTNode));
    memcpy_s(function_definition->body, sizeof(ASTNode), func_node, sizeof(ASTNode));

    add_function(tokenizer, function_definition);

    free_token(func_keyword);
    free_token(open_parenthesis);
    free_token(close_parenthesis);
    free_token(open_brace);
    free_token(close_bracket);
    debug_message("Added Function", TOP_LEVEL);
}