#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "error.h"
#include "lexer.h"
#include "free.h"
#include "parser.h"
#include "debug.h"
#include "data_structures/tokenizer.h"
#include "data_structures/ast.h"

#define STRING_BUFFER_LENGTH 256

void jakarta_cmd_read_file(const char* file_location) {
    debug_message("Begun Reading Input File", TOP_LEVEL);
    FILE* file_ptr = fopen(file_location, "r");
    if (file_ptr == NULL)
        jakarta_error(ERR_INVALID_FILE_NAME, NULL, file_location);
    char buffer[STRING_BUFFER_LENGTH];
    debug_message("Created Buffer", CREATION);
    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    AST* ast = create_ast();
    debug_message("Created Tokenizer", CREATION);
    unsigned int line_number = 1;
    while (fgets(buffer, sizeof(buffer), file_ptr))
        read_line(buffer, line_number++, tokenizer);
    // print_tokens(tokenizer);
    debug_message("Begun Parsing", TOP_LEVEL);
    while (tokenizer->tokens->length > 0)
        parse(tokenizer, ast->root);
    debug_message("Finished Parsing", TOP_LEVEL);
    print_ast_node(ast->root, 0);
    free_tokenizer(tokenizer);
    debug_message("Destroyed Tokenizer", REMOVAL);
    if (fclose(file_ptr) != 0)
        jakarta_error(ERR_CANNOT_CLOSE_FILE, NULL, file_location);
}

void jakarta_cmd_out_file(const char* file_location) {
    debug_message("Begun Opening Output File", TOP_LEVEL);
    FILE* file_ptr = fopen(file_location, "w");
    
    if (fclose(file_ptr) != 0)
        jakarta_error(ERR_CANNOT_CLOSE_FILE, NULL, file_location);
}