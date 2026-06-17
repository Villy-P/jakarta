#include "syntax.h"
#include "core.h"
#include "cmd.h"
#include "data_structures/tokenizer.h"

void parse_import(Tokenizer* tokenizer, ASTNode* ast_node) {
    peek_consume(tokenizer, KEYWORD_IMPORT);

    Token* string_literal = peek_consume(tokenizer, SYMBOL_STRING_LITERAL);

    // check first in the lib directory, then in the current directory
    char* lib_path = malloc(strlen("lib/.jk") + strlen(string_literal->content) + 1);
    sprintf(lib_path, "lib/%s.jk", string_literal->content);
    jakarta_cmd_read_file(lib_path);
    free(lib_path);

    peek_consume(tokenizer, SYMBOL_SEMICOLON);
}