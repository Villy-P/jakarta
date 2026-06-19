#include "syntax.h"
#include "data_structures/tokenizer.h"
#include "data_structures/compiler_state.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    if (peek(tokenizer, KEYWORD_TYPEDEF))
        parse_typedef(tokenizer, ast_node, state);
    else if (peek(tokenizer, KEYWORD_IMPORT))
        parse_import(tokenizer, ast_node, state);
    else if (peek(tokenizer, KEYWORD_FOR))
        parse_for(tokenizer, ast_node, state);
    else if (peek(tokenizer, KEYWORD_RET))
        parse_ret(tokenizer, ast_node, state);
    else if (peek(tokenizer, KEYWORD_FUNC))
        parse_func(tokenizer, ast_node, state);
    else if (peek(tokenizer, KEYWORD_IF))
        parse_if(tokenizer, ast_node, state);
    else if (peek(tokenizer, SYMBOL_IDENTIFIER))
        parse_expression(tokenizer, ast_node, state);
    else if (peek(tokenizer, KEYWORD_CLASS))
        parse_class(tokenizer, ast_node, state);
    else
        consume(tokenizer);
}