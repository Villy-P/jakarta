#include "parser.h"

void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node) {
    Token* type_token = peek_consume(tokenizer, SYMBOL_STRING);
    Token* name_token = peek_consume(tokenizer, SYMBOL_STRING);
    Type* type = get(tokenizer->type_symbol_tree, type_token->content);

    consume(tokenizer);
}