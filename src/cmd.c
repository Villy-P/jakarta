#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "error.h"
#include "lexer.h"
#include "tokenizer.h"
#include "free.h"
#include "parser.h"
#include "debug.h"
#include "ast.h"

#define STRING_BUFFER_LENGTH 256

void jakarta_cmd_read_file(const char* file_location) {
    debug_message("Begun Reading Input File", TOP_LEVEL);
    FILE* file_ptr = fopen(file_location, "r");
    if (file_ptr == NULL)
        jakarta_error_file_does_not_exist(file_location);
    char buffer[STRING_BUFFER_LENGTH];
    debug_message("Created Buffer", CREATION);
    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    AST* ast = create_ast();
    debug_message("Created Tokenizer", CREATION);
    while (fgets(buffer, sizeof(buffer), file_ptr))
        read_line(buffer, tokenizer);
    parse(tokenizer, ast->root);
    free_tokenizer(tokenizer);
    debug_message("Destroyed Tokenizer", REMOVAL);
    if (fclose(file_ptr) != 0)
        jakarta_error_cannot_close_file(file_location);
}

void jakarta_cmd_out_file(const char* file_location) {
    debug_message("Begun Opening Output File", TOP_LEVEL);
    FILE* file_ptr = fopen(file_location, "w");
    
    if (fclose(file_ptr) != 0)
        jakarta_error_cannot_close_file(file_location);
}