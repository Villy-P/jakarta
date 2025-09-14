#include "parser.h"
#include "debug.h"
#include "postfix.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node) {
    if (peek(tokenizer, KEYWORD_TYPEDEF))
        parse_typedef(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_FOR))
        parse_for(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_RET))
        parse_ret(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_FUNC))
        parse_func(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_IF))
        parse_if(tokenizer, ast_node);
    else if (peek_type(tokenizer)) 
        parse_variable(tokenizer, ast_node);
    else if (peek(tokenizer, SYMBOL_IDENTIFIER))
        parse_expression(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_CLASS))
        parse_class(tokenizer, ast_node);
    else
        consume(tokenizer);
}