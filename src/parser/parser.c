#include "parser.h"
#include "debug.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node) {
    printf("Parsing Token: %s\n", tokenizer->tokens[0]->content);
    if (peek(tokenizer, KEYWORD_TYPEDEF))
        parse_typedef(tokenizer, ast_node);
    else if (peek(tokenizer, KEYWORD_FUNC))
        parse_func(tokenizer, ast_node);
    else
        consume(tokenizer);
}