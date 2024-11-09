#include "parser.h"
#include "debug.h"

void parse(Tokenizer* tokenizer, ASTNode* ast_node) {
    debug_message("Begun Parsing", TOP_LEVEL);
    while (tokenizer->current_token_length > 0) {
        if (peek(tokenizer, KEYWORD_TYPEDEF))
            parse_typedef(tokenizer, ast_node);
        else if (peek(tokenizer, KEYWORD_FUNC))
            parse_func(tokenizer, ast_node);
        else
            consume(tokenizer);
    }
    debug_message("Finished Parsing", TOP_LEVEL);
}