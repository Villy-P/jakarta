#include <stdio.h>

#include "parser.h"
#include "debug.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node) {
    printf("Parsing node with identifier: %d\n", ((Token*)(tokenizer->tokens->data[0]))->symbol);
    if (peek(tokenizer, KEYWORD_TYPEDEF))
        parse_typedef(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_RET)) {
        parse_ret(tokenizer, ast_node);
    } else if (peek(tokenizer, KEYWORD_FUNC))
        parse_func(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_IF))
        parse_if(tokenizer, ast_node);
    else if (peek_type(tokenizer)) 
        parse_variable(tokenizer, ast_node);
    else
        consume(tokenizer);
}