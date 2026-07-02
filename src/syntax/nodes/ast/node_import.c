#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"

#include "debug.h"
#include "syntax.h"


void parse_import(Tokenizer* tokenizer, ASTNode* ast_node, CompilerState* state) {
    if (ast_node->identifier != AST_IDENTIFIER_BASE_PROGRAM) {
        jakarta_error(ERR_CUSTOM, nullptr, "Import statements must be at the top level");
    }

    Token* import_keyword = consume(tokenizer);
    Token* module_token = peek_consume(tokenizer, SYMBOL_STRING_LITERAL);

    ASTNode* import_node = create_ast_node(AST_IDENTIFIER_IMPORT_STATEMENT, module_token);
    ds_astnode_ptr_array_push(ast_node->nodes, import_node);

    ds_char_ptr_array_push(&state->files_to_parse, module_token->content);

    log_msg(logs.main, "[IMPORT] Added import statement: %s", module_token->content);

    free_token(import_keyword);
}
