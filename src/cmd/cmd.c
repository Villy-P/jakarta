#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "core.h"
#include "syntax.h"
#include "debug.h"
#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"

static Tokenizer* tokenize_file(FILE* file);
static AST* parse_tokens(Tokenizer* tokenizer);
static FILE* open_file_read(const char* path);
static FILE* open_file_write(const char* path);
static void close_file(FILE* f, const char* path);

void jakarta_cmd_read_file(const char* file_location) {
    debug_message("Begun Reading Input File", TOP_LEVEL);
    FILE* file_ptr = open_file_read(file_location);

    debug_message("Created Tokenizer", CREATION);
    Tokenizer* tokenizer = tokenize_file(file_ptr);

    debug_message("Begun Parsing", TOP_LEVEL);
    AST* ast = parse_tokens(tokenizer);

    print_ast_node(ast->root, 0);

    free_tokenizer(tokenizer);
    debug_message("Destroyed Tokenizer", REMOVAL);

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

static Tokenizer* tokenize_file(FILE* file) {
    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    char buffer[STRING_BUFFER_LENGTH];
    unsigned int line_number = 1;
    while (fgets(buffer, sizeof(buffer), file))
        read_line(buffer, line_number++, tokenizer);
    return tokenizer;
}

static AST* parse_tokens(Tokenizer* tokenizer) {
    AST* ast = create_ast();
    while (tokenizer->tokens->length > 0)
        parse(tokenizer, ast->root);
    return ast;
}
