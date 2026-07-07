#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/tokenizer.h"
#include "symbol.h"
#include "syntax.h"

void parse_class(Tokenizer* tokenizer, ASTNode* ast_node,
                 CompilerState* state) {
    consume(tokenizer);

    Token* name = consume(tokenizer);
    consume(tokenizer);

    ASTNode* class_node = create_ast_node(AST_IDENTIFIER_CLASS_CREATOR, name);
    ASTNode* class_body = create_ast_node(AST_IDENTIFIER_CLASS_BODY, nullptr);

    while (!peek(tokenizer, SYMBOL_CLOSE_BRACE)) {
        if (peek_type(tokenizer)) {
            parse_variable(tokenizer, class_body);
        } else if (peek(tokenizer, KEYWORD_FUNC)) {
            parse_func(tokenizer, class_body, state);
        } else {
            jakarta_error(ERR_INVALID_TOKEN, consume(tokenizer),
                          "variable or function");
        }
    }

    ds_astnode_ptr_array_push(class_node->nodes, class_body);
    ds_astnode_ptr_array_push(ast_node->nodes, class_node);
}