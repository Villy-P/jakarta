#include <regex.h>
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

void read_line(char* line, unsigned int line_number, Tokenizer* tokenizer) {
    unsigned int col = 1;
    while (strcmp(line, "\0") != STRING_UNEQUAL) {
        if (line[0] == '/' && line[1] == '/')
            break;
        if (line[0] == '"') {
            char* str_lit = get_string_literal(&line);
            printf("STRING: %s\n", str_lit);
            Token* token = create_token(SYMBOL_STRING_LITERAL, line_number, col, str_lit);
            col += strlen(str_lit) + 2;
            add_to_array(tokenizer->tokens, token);
            fprintf(logs.tokens, "Token %d (%d, %d): %s\n", token->symbol, token->line, token->col, token->content);
            free(str_lit);
            continue;
        }
        char* str = get_string(&line);
        if (str != NULL) {
            Symbol symbol = get_keyword_from_str(str);
            Token* token = create_token(symbol, line_number, col, str);
            col += strlen(str);
            add_to_array(tokenizer->tokens, token);
            fprintf(logs.tokens, "Token %d (%d, %d): %s\n", token->symbol, token->line, token->col, token->content);
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
            Token* token_obj = create_token(token_symbol, line_number, col, token_ptr);
            add_to_array(tokenizer->tokens, token_obj);
            fprintf(logs.tokens, "Token %d (%d, %d): %s\n", token_obj->symbol, token_obj->line, token_obj->col, token_obj->content);
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
    regex_t reegex;
    regmatch_t groups[MAX_REGEX_GROUPS];
    int value = 0;

    value = regcomp(&reegex, "^(\\w+|=[=!><]|[\\+\\-\\*\\/%&\\|\\^]=|\\+\\+|--|<<|>>>?|&&?|\\|\\|?|>=|<=|!|!=|\\[\\])", REG_EXTENDED);
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
