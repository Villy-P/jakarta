#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

#define INITIAL_TOKENS_LENGTH 64
#define MAX_REGEX_GROUPS 1
#define STRING_UNEQUAL 1
#define REGEX_FLAGS 0

Token** tokens = NULL;
unsigned int token_length = INITIAL_TOKENS_LENGTH;

void read_line(char* line) {
    if (tokens == NULL)
        tokens = malloc(sizeof(Token*) * INITIAL_TOKENS_LENGTH);
    printf("Line: %s %d\n", line, sizeof(Token*));
    while (strcmp(line, "\0") >= STRING_UNEQUAL) {
        char* str = get_string(&line);
        if (str != NULL) {
            printf("str:   %s\n", str);
            Token* token = create_token(SYMBOL_STRING, 0, 0, str);
        } else {
            char token = line[0];
            printf("other: %c\n", token);
            line += 1;
        }
    }
    return;
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