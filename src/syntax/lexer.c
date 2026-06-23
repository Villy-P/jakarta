#include "symbol.h"
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "data_structures/tokenizer.h"
#include "debug.h"
#include "syntax.h"

#define MAX_REGEX_GROUPS 1
#define STRING_UNEQUAL 0
#define REGEX_FLAGS 0

void read_line(char* line, const char* file_name, uint32_t line_number, Tokenizer* tokenizer) {
    uint32_t col = 1;
    while (strcmp(line, "\0") != STRING_UNEQUAL) {
        if (line[0] == '/' && line[1] == '/') {
            break;
        }
        if (line[0] == '"') {
            char* str_lit = get_string_literal(&line);
            Token* token = create_token(SYMBOL_STRING_LITERAL, line_number, col, str_lit, file_name);
            col += strlen(str_lit) + 2;
            add_to_array(tokenizer->tokens, token);
            log_msg(logs.main, "[TOKEN] Added string literal %d @ (%d, %d): %s", token->symbol, token->line, token->col, token->content);

            free(str_lit);
            continue;
        }
        char* str = get_string(&line);
        if (str != NULL) {
            Symbol symbol = get_keyword_from_str(str);
            Token* token = create_token(symbol, line_number, col, str, file_name);
            col += strlen(str);
            add_to_array(tokenizer->tokens, token);
            log_msg(logs.main, "[TOKEN] Added string token %d @ (%d, %d): %s", token->symbol, token->line, token->col, token->content);
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
            log_msg(logs.main, "[TOKEN] Added non-string token %d @ (%d, %d): %s", token_obj->symbol, token_obj->line, token_obj->col, token_obj->content);
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
        if ((*line)[0] == '\\') {
            (*line)++;
        }
        (*line)++;
        length++;
    }

    if ((*line)[0] == '"') {
        char* str = calloc(length + 1, sizeof(char));
        memcpy(str, start, length);
        (*line)++;
        return str;
    } 
    jakarta_error(ERR_UNTERMINATED_STRING, NULL, start);
    return NULL;
}

char* get_string(char** line) {
    pcre2_code *regex = NULL;
    int errornumber = 0;
    PCRE2_SIZE erroroffset = 0;
    PCRE2_SPTR pattern = (PCRE2_SPTR)"^(\\d+\\.?\\d*|\\w+|=[=!><]|[\\+\\-\\*\\/%&\\|\\^]=|\\+\\+|--|<<|>>>?|&&?|\\|\\|?|>=|<=|!|!=|\\[\\])";

    // Compile the pattern
    regex = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (!regex) {
        return NULL; // Compilation failed
    }

    // Create match data
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(regex, NULL);
    
    // Perform match
    int rcmatch = pcre2_match(regex, (PCRE2_SPTR)*line, strlen(*line), 0, 0, match_data, NULL);
    
    char* str = NULL;
    if (rcmatch > 0) {
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        size_t match_len = ovector[1] - ovector[0];
        str = calloc(match_len + 1, sizeof(char));
        memcpy(str, *line + ovector[0], match_len);
        
        *line += match_len; // Advance the pointer
    }

    // Cleanup
    pcre2_match_data_free(match_data);
    pcre2_code_free(regex);

    return str;
}

bool is_number_symbol(char* str) {
    pcre2_code *regex = NULL;
    int errornumber = 0;
    PCRE2_SIZE erroroffset = 0;
    
    // -?(0x[0-9A-Fa-f]+|0b[01]+|0[0-7]+|([0-9]*\\.[0-9]+|[0-9]+\\.?)([eE][+-]?[0-9]+)?)
    PCRE2_SPTR pattern = (PCRE2_SPTR)"^[0-9]+\\.?[0-9]*$";
    
    regex = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (!regex) {
        return false;
    }
    
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(regex, NULL);
    int rcmatch = pcre2_match(regex, (PCRE2_SPTR)str, strlen(str), 0, 0, match_data, NULL);
    
    pcre2_code_free(regex);

    return rcmatch >= 0;
}