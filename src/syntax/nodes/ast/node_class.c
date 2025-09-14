#include <stdlib.h>
#include <stdio.h>

#include "syntax.h"
#include "core.h"

void parse_class(Tokenizer* tokenizer, ASTNode* ast_node) {
    add_scope(tokenizer);

    consume(tokenizer);

    Token* name = consume(tokenizer);
    add_type(tokenizer, create_type(name->content, 0, CLASS));

    consume(tokenizer);

    ASTNode* class_node = create_ast_node(AST_IDENTIFIER_CLASS_CREATOR, name);
    ASTNode* class_body = create_ast_node(AST_IDENTIFIER_CLASS_BODY, NULL);

    ClassDefinition* class_definition = create_class_definition(name->content);
    add_class(tokenizer, class_definition);
    tokenizer->current_class = class_definition->name;

    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE)) {
        if (peek_type(tokenizer)) 
            parse_variable(tokenizer, class_body);
        else if (peek(tokenizer, KEYWORD_FUNC))
            parse_func(tokenizer, class_body);
        else
            jakarta_error(ERR_INVALID_TOKEN, consume(tokenizer), "variable or function");
    }

    add_to_array(class_node->nodes, class_body);
    add_to_array(ast_node->nodes, class_node);
}