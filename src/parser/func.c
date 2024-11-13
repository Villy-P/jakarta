#include <stdlib.h>

#include "parser.h"
#include "error.h"
#include "free.h"
#include "debug.h"

void parse_func(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* func_keyword = consume(tokenizer);
    Token* func_type = peek_consume(tokenizer, SYMBOL_STRING);
    Token* func_name = peek_consume(tokenizer, SYMBOL_STRING);
    Token* open_parenthesis = peek_consume(tokenizer, SYMBOL_OPEN_PARENTHESIS);

    ASTNode* func_node = create_ast_node(AST_IDENTIFIER_FUNCTION_DEFINITION, func_name);
    ASTNode* func_type_node = create_ast_node(AST_IDENTIFIER_FUNCTION_RETURN_TYPE, func_type);
    ASTNode* func_params_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETERS, NULL);
    ASTNode* func_body_node = create_ast_node(AST_IDENTIFIER_FUNCTION_BODY, NULL);

    add_ast_node(func_node, func_type_node);
    add_ast_node(func_node, func_params_node);

    while (!peek(tokenizer, SYMBOL_CLOSE_PARENTHESIS)) {
        Token* arg_type = peek_consume(tokenizer, SYMBOL_STRING);
        Token* arg_name = peek_consume(tokenizer, SYMBOL_STRING);
        Token* comma = peek(tokenizer, SYMBOL_COMMA) ? consume(tokenizer) : NULL;
        Type* argtype = get_type(tokenizer, arg_type->content);
        
        if (argtype == NULL)
            jakarta_error_undefined_identifier(arg_type);

        ASTNode* arg_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETER, arg_name);
        ASTNode* arg_type_node = create_ast_node(AST_IDENTIFIER_FUNCTION_PARAMETER_TYPE, arg_type);

        add_ast_node(arg_node, arg_type_node);
        add_ast_node(func_params_node, arg_node);

        if (comma != NULL)
            free_token(comma);
    }

    Token* close_parenthesis = consume(tokenizer);
    Token* open_brace = peek_consume(tokenizer, SYMBOL_OPEN_BRACE);
    Type* type = get_type(tokenizer, func_type->content);
    if (type == NULL)
        jakarta_error_undefined_identifier(func_type);

    add_ast_node(func_node, func_body_node);
    add_ast_node(ast_node, func_node);

    free_token(func_keyword);
    free_token(open_parenthesis);
    free_token(close_parenthesis);
    free_token(open_brace);

    debug_message("Added Function", TOP_LEVEL);
}