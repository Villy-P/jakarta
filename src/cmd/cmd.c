#include "cmd.h"

#include <stdio.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/tokenizer.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "syntax.h"

static ASTNode* parse_tokens(Tokenizer* tokenizer, CompilerState* state);
static FILE* open_file_read(const char* path);
static FILE* open_file_write(const char* path);
static void close_file(FILE* file, const char* path);
static void process(const char* file_location, CompilerState* state);

void jakarta_cmd_read_file(const char* file_location, CompilerState* state) {
    ds_char_ptr_array_push(&state->files_to_parse, file_location);

    while (state->files_to_parse.length > 0) {
        log_msg(logs.main, "[IMPORT] Files left to parse: %d",
                state->files_to_parse.length);
        char* base_file_path = ds_char_ptr_array_get(&state->files_to_parse, 0);
        printf("DEBUG: The pointer retrieved is: %p\n", base_file_path);

        log_msg(logs.main, "[IMPORT] Parsing imported file: %s",
                base_file_path);
        process(base_file_path, state);

        ds_char_ptr_array_remove(&state->files_to_parse, 0, nullptr);
        log_msg(logs.main, "[IMPORT] Finished processing file: %s",
                base_file_path);
    }

    gather_declarations(state);
    resolve_types(state);
}

static void process(const char* file_location, CompilerState* state) {
    FILE* file_ptr = open_file_read(file_location);
    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    tokenize_file(file_ptr, file_location, tokenizer);

    ASTNode* ast_root = parse_tokens(tokenizer, state);
    log_msg(logs.main, "[AST] Finished parsing tokens into AST for file: %s",
            file_location);
    ForestEntry* entry = create_forest_entry(file_location, ast_root);
    ds_forest_entry_ptr_array_push(&state->forest, entry);
    log_msg(logs.main, "[AST] Added AST to forest for file: %s", file_location);

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
    if (!file) {
        handle_error(ERROR_INVALID_FILE_NAME, nullptr, nullptr, path);
        return nullptr;
    }
    return file;
}

static FILE* open_file_write(const char* path) {
    FILE* file = fopen(path, "w");
    if (!file) {
        handle_error(ERROR_INVALID_FILE_NAME, nullptr, nullptr, path);
        return nullptr;
    }
    return file;
}

static void close_file(FILE* file, const char* path) {
    if (fclose(file) != 0) {
        jakarta_error(ERR_CANNOT_CLOSE_FILE, nullptr, path);
    }
}

void tokenize_file(FILE* file, const char* file_location,
                   Tokenizer* tokenizer) {
    log_msg(logs.main, "[TOKENIZER] Tokenizing new file");
    char buffer[STRING_BUFFER_LENGTH];
    unsigned int line_number = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        read_line(buffer, file_location, line_number++, tokenizer);
    }
}

static ASTNode* parse_tokens(Tokenizer* tokenizer, CompilerState* state) {
    log_msg(logs.main, "[AST] Parsing tokens into AST: %d tokens",
            tokenizer->tokens.length);
    ASTNode* ast_root = create_ast_node(AST_IDENTIFIER_BASE_PROGRAM, nullptr);
    while (tokenizer->tokens.length > 0) {
        parse(tokenizer, ast_root, state);
    }
    return ast_root;
}
