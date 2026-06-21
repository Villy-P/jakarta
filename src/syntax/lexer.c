#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "syntax.h"
#include "core.h"
#include "data_structures/tokenizer.h"
#include "debug.h"

#define MAX_REGEX_GROUPS 1
#define STRING_UNEQUAL 0
#define REGEX_FLAGS 0

void read_line(char* line, char* file_name, unsigned int line_number, Tokenizer* tokenizer) {
    unsigned int col = 1;
    while (strcmp(line, "\0") != STRING_UNEQUAL) {
        if (line[0] == '/' && line[1] == '/')
            break;
        if (line[0] == '"') {
            char* str_lit = get_string_literal(&line);
            Token* token = create_token(SYMBOL_STRING_LITERAL, line_number, col, str_lit, file_name);
            col += strlen(str_lit) + 2;
            add_to_array(tokenizer->tokens, token);
            log_msg(logs.main, "[TOKEN] Added string literal %d @ (%d, %d): %s\n", token->symbol, token->line, token->col, token->content);

            free(str_lit);
            continue;
        }
        char* str = get_string(&line);
        if (str != NULL) {
            Symbol symbol = get_keyword_from_str(str);
            Token* token = create_token(symbol, line_number, col, str, file_name);
            col += strlen(str);
            add_to_array(tokenizer->tokens, token);
            log_msg(logs.main, "[TOKEN] Added token %d @ (%d, %d): %s\n", token->symbol, token->line, token->col, token->content);
        } else {
            char token = line[0];
            if (token == ' ' || token == '\n') {
                line++;
                col++;
                continue;
            }
            char* token_ptr = malloc(2 * sizeof(char));
            token_ptr[0] = token;
            token_ptr[1] = '\0';
            Symbol token_symbol = get_symbol_from_char(token);
            Token* token_obj = create_token(token_symbol, line_number, col, token_ptr, file_name);
            add_to_array(tokenizer->tokens, token_obj);
            log_msg(logs.main, "[TOKEN] Added token %d @ (%d, %d): %s\n", token_obj->symbol, token_obj->line, token_obj->col, token_obj->content);
            line++;
            col++;
            free(token_ptr);
        }
        free(str);
    }
}

char* get_string_literal(char** line) {
    (*line)++;
    char* start = *line;
    size_t length = 0;

    while ((*line)[0] != '"' && (*line)[0] != '\0') {
        if ((*line)[0] == '\\')
            (*line)++;
        (*line)++;
        length++;
    }

    if ((*line)[0] == '"') {
        char* str = calloc(length + 1, sizeof(char));
        memcpy(str, start, length);
        (*line)++;
        return str;
    } else {
        jakarta_error(ERR_UNTERMINATED_STRING, NULL, start);
        return NULL;
    }
}

char* get_string(char** line) {
    pcre2_code *re;
    int errornumber;
    PCRE2_SIZE erroroffset;
    PCRE2_SPTR pattern = (PCRE2_SPTR)"^(\\w+|=[=!><]|[\\+\\-\\*\\/%&\\|\\^]=|\\+\\+|--|<<|>>>?|&&?|\\|\\|?|>=|<=|!|!=|\\[\\])";

    // Compile the pattern
    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (!re) return NULL; // Compilation failed

    // Create match data
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);
    
    // Perform match
    int rc = pcre2_match(re, (PCRE2_SPTR)*line, strlen(*line), 0, 0, match_data, NULL);
    
    char* str = NULL;
    if (rc > 0) {
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        size_t match_len = ovector[1] - ovector[0];
        str = calloc(match_len + 1, sizeof(char));
        memcpy(str, *line + ovector[0], match_len);
        
        *line += match_len; // Advance the pointer
    }

    // Cleanup
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    return str;
}