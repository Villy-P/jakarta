#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "error.h"
#include "parser.h"
#include "tokenizer.h"

#define STRING_BUFFER_LENGTH 256

void jakarta_cmd_read_file(const char* file_location) {
    FILE* file_ptr = fopen(file_location, "r");
    if (file_ptr == NULL)
        jakarta_error_file_does_not_exist(file_location);
    char buffer[STRING_BUFFER_LENGTH];
    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    while (fgets(buffer, sizeof(buffer), file_ptr))
        read_line(buffer, tokenizer);
    print_tokens(tokenizer);
    free(tokenizer);
    if (fclose(file_ptr) != 0)
        jakarta_error_cannot_close_file(file_location);
}

void jakarta_cmd_out_file(const char* file_location) {
    FILE* file_ptr = fopen(file_location, "w");
    
    if (fclose(file_ptr) != 0)
        jakarta_error_cannot_close_file(file_location);
}