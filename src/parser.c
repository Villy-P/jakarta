#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

#define INITIAL_TOKENS_LENGTH 64
#define MAX_REGEX_GROUPS 1
#define REGEX_FLAGS 0

Token** tokens = NULL;
unsigned int token_length = INITIAL_TOKENS_LENGTH;

void read_line(char* line) {
    if (tokens == NULL)
        tokens = malloc(sizeof(Token) * INITIAL_TOKENS_LENGTH);
    return;
}

void get_string(char* line) {
    regex_t reegex;
    regmatch_t groups[MAX_REGEX_GROUPS];
    int value;
    value = regcomp(&reegex, "^(\\w+)", REG_EXTENDED);
    value = regexec(&reegex, line, MAX_REGEX_GROUPS, groups, REGEX_FLAGS);

    if (value == REG_OK) {
        printf("%s\n", line);
        for (unsigned int g = 0; g < MAX_REGEX_GROUPS; g++){
            if (groups[g].rm_so == (size_t)-1)
                break;
            char sourceCopy[strlen(line) + 1];
            strcpy(sourceCopy, line);
            sourceCopy[groups[g].rm_eo] = 0;
            char* str = sourceCopy + groups[g].rm_so;
            printf("Group %u: [%2u-%2u]: %s\n",
                    g, groups[g].rm_so, groups[g].rm_eo,
                    str);
        }
    } else if (value == REG_NOMATCH) {
        printf("No line detected");
    }

    regfree(&reegex);
}