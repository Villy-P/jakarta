#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "error.h"
#include "token.h"
#include "tokenizer.h"

#define MAX_REGEX_GROUPS 1
#define STRING_UNEQUAL 0
#define REGEX_FLAGS 0

void read_line(char* line, unsigned int line_number, Tokenizer* tokenizer) {
    while (strcmp(line, "\0") != STRING_UNEQUAL) {
        char* str = get_string(&line);
        if (str != NULL) {
            Symbol symbol = get_keyword_from_str(str);
            Token* token = create_token(symbol, line_number, 0, str);
            add_token(tokenizer, token);
        } else {
            char token = line[0];
            if (token == ' ' || token == '\n') {
                line += 1;
                continue;
            }
            char* token_ptr = malloc(2 * sizeof(char));
            token_ptr[0] = token;
            token_ptr[1] = '\0';
            Symbol token_symbol = get_symbol_from_char(token);
            Token* token_obj = create_token(token_symbol, line_number, 0, token_ptr);
            add_token(tokenizer, token_obj);
            line += 1;
            free(token_ptr);
        }
        free(str);
    }
}

char* get_string(char** line) {
    regex_t reegex;
    regmatch_t groups[MAX_REGEX_GROUPS];
    int value = 0;
    value = regcomp(&reegex, "^(\\w+)", REG_EXTENDED);
    value = regexec(&reegex, *line, MAX_REGEX_GROUPS, groups, REGEX_FLAGS);

    char* str = NULL;
    size_t size = 1;

    if (value != REG_NOMATCH) {
        for (unsigned int g = 0; g < MAX_REGEX_GROUPS; g++){
            if ((size_t)groups[g].rm_so == (size_t) - 1)
                break;
            size_t substr_len = (size_t)groups[g].rm_eo;
            str = calloc(sizeof(char), substr_len + 1);
            memcpy(str, *line, substr_len);
            size = substr_len;
        }
    } else {
        regfree(&reegex);
        return NULL;
    }

    regfree(&reegex);

    *line += size;

    return str;
}
