#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "error.h"

#define INITIAL_TOKENS_LENGTH 64
#define MAX_REGEX_GROUPS 1
#define STRING_UNEQUAL 1
#define REGEX_FLAGS 0

Token** tokens = NULL;
unsigned int max_token_length = INITIAL_TOKENS_LENGTH;
unsigned int current_token_length = 0;

void read_line(char* line) {
    if (tokens == NULL)
        tokens = malloc(sizeof(Token*) * INITIAL_TOKENS_LENGTH);
    printf("Line: %s\n", line);
    while (strcmp(line, "\0") >= STRING_UNEQUAL) {
        char* str = get_string(&line);
        if (str != NULL) {
            printf("str:   %s\n", str);
            Token* token = create_token(SYMBOL_STRING, 0, 0, str);
            add_token(token);
        } else {
            char token = line[0];
            printf("other: %c\n", token);
            Symbol token_symbol = get_symbol_from_char(token);
            Token* token_obj = create_token(token_symbol, 0, 0, &token);
            add_token(token_obj);
            line += 1;
        }
    }
    return;
}

void add_token(Token* token) {
    if (current_token_length <= max_token_length) {
        tokens[current_token_length] = token;
        current_token_length++;
    } else {
        max_token_length *= 2;
        tokens = realloc(tokens, max_token_length * sizeof(Token*));
        tokens[current_token_length] = token;
        current_token_length++;
    }
}

char* get_string(char** line) {
    regex_t reegex;
    regmatch_t groups[MAX_REGEX_GROUPS];
    int value;
    value = regcomp(&reegex, "^(\\w+)", REG_EXTENDED);
    value = regexec(&reegex, *line, MAX_REGEX_GROUPS, groups, REGEX_FLAGS);

    char* str = NULL;

    if (value == REG_OK) {
        for (unsigned int g = 0; g < MAX_REGEX_GROUPS; g++){
            if (groups[g].rm_so == (size_t) - 1)
                break;
            char sourceCopy[strlen(*line) + 1];
            strcpy(sourceCopy, *line);
            sourceCopy[groups[g].rm_eo] = 0;
            str = sourceCopy + groups[g].rm_so;
        }
    } else {
        return NULL;
    }

    regfree(&reegex);

    int str_length = strlen(str);
    *line += str_length;

    return str;
}

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content) {
    Token* item = malloc(sizeof(Token));
    item->symbol = symbol;
    item->line = line;
    item->col = col;
    item->content = malloc(sizeof(char));
    strcpy(item->content, content);
    return item;
}

Symbol get_symbol_from_char(char ch) {
    switch (ch) {
        case '`': return SYMBOL_BACKTICK;
        case '~': return SYMBOL_TILDE;
        case '!': return SYMBOL_EXCLAMATION;
        case '@': return SYMBOL_AT;
        case '#': return SYMBOL_HASHTAG;
        case '$': return SYMBOL_DOLLAR;
        case '%': return SYMBOL_PERCENT;
        case '^': return SYMBOL_CARROT;
        case '&': return SYMBOL_AMPERSAND;
        case '*': return SYMBOL_ASTERISK;
        case '(': return SYMBOL_OPEN_PARENTHESIS;
        case ')': return SYMBOL_CLOSE_PARENTHESIS;
        case '-': return SYMBOL_DASH;
        case '_': return SYMBOL_UNDERSCORE;
        case '+': return SYMBOL_PLUS;
        case '=': return SYMBOL_EQUALS;
        case '[': return SYMBOL_OPEN_BRACKET;
        case ']': return SYMBOL_CLOSE_BRACKET;
        case '{': return SYMBOL_OPEN_BRACE;
        case '}': return SYMBOL_CLOSE_BRACE;
        case '|': return SYMBOL_VERTICAL_LINE;
        case ':': return SYMBOL_COLON;
        case ';': return SYMBOL_SEMICOLON;
        case '"': return SYMBOL_QUESTION;
        case '<': return SYMBOL_OPEN_ANGLE;
        case '>': return SYMBOL_CLOSE_ANGLE;
        case ',': return SYMBOL_COMMA;
        case '.': return SYMBOL_PERIOD;
        case '?': return SYMBOL_QUESTION;
        case '/': return SYMBOL_SLASH;
        case ' ': return SYMBOL_SPACE;
        case '\n': return SYMBOL_NEWLINE;
        case '\\': return SYMBOL_BACKSLASH;
        case '\'': return SYMBOL_APOSTRAPHE;
    };
    jakarta_error_unknown_symbol(ch);
    return SYMBOL_NONE;
}