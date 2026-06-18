#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "core.h"
#include "syntax.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"

static ASTNode* parse_tokens(Tokenizer* tokenizer, CompilerState* state);
static FILE* open_file_read(const char* path);
static FILE* open_file_write(const char* path);
static void close_file(FILE* f, const char* path);
static void process(const char* file_location, CompilerState* state);

void jakarta_cmd_read_file(const char* file_location) {
    CompilerState* state = create_compiler_state();
    
    add_to_array(state->files_to_parse, file_location);

    while (state->files_to_parse->length > 0) {
        log_msg(logs.main, "[IMPORT] Files left to parse: %d\n", state->files_to_parse->length);
        char* base_file_path = (char*)get_from_array(state->files_to_parse, 0);

        log_msg(logs.main, "[IMPORT] Parsing imported file: %s\n", base_file_path);
        process(base_file_path, state);

        remove_from_array(state->files_to_parse, 0);
        log_msg(logs.main, "[IMPORT] Finished processing file: %s\n", base_file_path);
    }

    gather_declarations(state);
}

static void process(const char* file_location, CompilerState* state) {
    FILE* file_ptr = open_file_read(file_location);
    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    tokenize_file(file_ptr, tokenizer);

    ASTNode* ast_root = parse_tokens(tokenizer, state);
    log_msg(logs.main, "[AST] Finished parsing tokens into AST for file: %s\n", file_location);
    ForestEntry* entry = create_forest_entry(file_location, ast_root);
    add_to_array(state->forest, entry);
    log_msg(logs.main, "[AST] Added AST to forest for file: %s\n", file_location);

    print_ast_node(ast_root, "", true);

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
        handle_error(ERROR_INVALID_FILE_NAME, NULL, NULL, path);
    return file;
}

static FILE* open_file_write(const char* path) {
    FILE* file = fopen(path, "w");
    if (!file)
        handle_error(ERROR_INVALID_FILE_NAME, NULL, NULL, path);
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

static ASTNode* parse_tokens(Tokenizer* tokenizer, CompilerState* state) {
    log_msg(logs.main, "[AST] Parsing tokens into AST: %d tokens\n", tokenizer->tokens->length);
    ASTNode* ast_root = create_ast_node(AST_IDENTIFIER_BASE_PROGRAM, NULL);
    while (tokenizer->tokens->length > 0)
        parse(tokenizer, ast_root, state);
    return ast_root;
}
