#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "core.h"
#include "syntax.h"
#include "debug.h"
#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"

static AST* parse_tokens(Tokenizer* tokenizer, CompilerState* state, const char* file_location);
static FILE* open_file_read(const char* path);
static FILE* open_file_write(const char* path);
static void close_file(FILE* f, const char* path);

void jakarta_cmd_read_file(const char* file_location) {
    FILE* file_ptr = open_file_read(file_location);

    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    tokenize_file(file_ptr, tokenizer);

    debug_message("Begun Parsing", TOP_LEVEL);
    CompilerState* state = create_compiler_state();
    AST* ast = parse_tokens(tokenizer, state, file_location);

    print_ast_node(ast->root, "", true);

    for (unsigned int i = 0; i < state->files_to_parse->length; ++i) {
        char* base_file_path = (char*)get_from_array(state->files_to_parse, i);

        char* base_file_path_with_extension = malloc(strlen(base_file_path) + strlen("lib/.jk") + 1);
        strcpy(base_file_path_with_extension, "lib/");
        strcat(base_file_path_with_extension, base_file_path);
        strcat(base_file_path_with_extension, ".jk");

        log_msg(logs.main, "[IMPORT] Parsing imported file: %s\n", base_file_path_with_extension);
        jakarta_cmd_read_file(base_file_path_with_extension);
    }

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

static AST* parse_tokens(Tokenizer* tokenizer, CompilerState* state, const char* file_location) {
    log_msg(logs.main, "[AST] Parsing tokens into AST: %d tokens\n", tokenizer->tokens->length);
    AST* ast = create_ast();
    insert(state->forest, file_location, ast);
    while (tokenizer->tokens->length > 0)
        parse(tokenizer, ast->root, state);
    return ast;
}
