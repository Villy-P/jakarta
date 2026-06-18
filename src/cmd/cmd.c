#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "core.h"
#include "syntax.h"
#include "debug.h"
#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"

static AST* parse_tokens(Tokenizer* tokenizer, char* file_location);
static FILE* open_file_read(const char* path);
static FILE* open_file_write(const char* path);
static void close_file(FILE* f, const char* path);

void jakarta_cmd_read_file(const char* file_location, Tokenizer* tokenizer) {
    debug_message("Begun Reading Input File", TOP_LEVEL);
    FILE* file_ptr = open_file_read(file_location);

    debug_message("Created Tokenizer", CREATION);
    tokenize_file(file_ptr, tokenizer);

    debug_message("Begun Parsing", TOP_LEVEL);
    AST* ast = parse_tokens(tokenizer, file_location);

    print_ast_node(ast->root, "", true);

    close_file(file_ptr, file_location);
}

void jakarta_cmd_out_file(const char* file_location) {
    debug_message("Begun Opening Output File", TOP_LEVEL);
    FILE* file_ptr = open_file_write(file_location);

    close_file(file_ptr, file_location);
}

static FILE* open_file_read(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file)
        jakarta_error(ERR_INVALID_FILE_NAME, NULL, path);
    return file;
}

static FILE* open_file_write(const char* path) {
    FILE* file = fopen(path, "w");
    if (!file)
        jakarta_error(ERR_INVALID_FILE_NAME, NULL, path);
    return file;
}

static void close_file(FILE* f, const char* path) {
    if (fclose(f) != 0)
        jakarta_error(ERR_CANNOT_CLOSE_FILE, NULL, path);
}

void tokenize_file(FILE* file, Tokenizer* tokenizer) {
    log_msg(logs.main, "[TOKENIZER] Tokenizing new file\n");
    char buffer[STRING_BUFFER_LENGTH];
    unsigned int line_number = 1;
    while (fgets(buffer, sizeof(buffer), file))
        read_line(buffer, line_number++, tokenizer);
}

static AST* parse_tokens(Tokenizer* tokenizer, char* file_location) {
    log_msg(logs.main, "[AST] Parsing tokens into AST: %d tokens\n", tokenizer->tokens->length);
    AST* ast = create_ast();
    CompilerState* state = create_compiler_state();
    insert(state->forest, file_location, ast);
    while (tokenizer->tokens->length > 0)
        parse(tokenizer, ast->root, state);
    return ast;
}
