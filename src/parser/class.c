#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "free.h"
#include "postfix.h"

void parse_class(Tokenizer* tokenizer, ASTNode* ast_node) {
    add_scope(tokenizer);
    consume(tokenizer);
    Token* name = consume(tokenizer);
    add_type(tokenizer, create_type(name->content, 0, CLASS));
    
    if (peek_type(tokenizer)) 
        parse_variable(tokenizer, ast_node);
}