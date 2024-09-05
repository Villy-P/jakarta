#include <regex.h>
#include <stdio.h>

#include "parser.h"

void read_line(char* line) {
    regex_t reegex;
    int value;
    value = regcomp(&reegex, "^\\w+", REG_EXTENDED);
    value = regexec(&reegex, line, 0, NULL, 0);

    if (value == REG_OK) {
        printf("%s", line);
    } else if (value == REG_NOMATCH) {
        printf("No line detected");
    }
}