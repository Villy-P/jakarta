#include <stdlib.h>

#include "core.h"
#include "syntax.h"
#include "data_structures/symbol_table.h"

void parse_typedef(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM)
        jakarta_error_invalid_typedef_location(consume(tokenizer));

    Token* typedef_keyword = consume(tokenizer);
    Token* type_alias = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Token* type_name = peek_consume(tokenizer, SYMBOL_IDENTIFIER);
    Type* type = malloc(sizeof(Type));
    TypeAlias* alias = create_type_alias(type_alias->content, type);
    Token* semicolon = peek_consume(tokenizer, SYMBOL_SEMICOLON);

    free_token(typedef_keyword);
    free_token(semicolon);
    free(type);
}
