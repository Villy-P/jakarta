#include <regex.h>
#include <stdio.h>

#include "parser.h"

#define REGEX_SUCCESSFUL 0

void read_line(char* line) {
    regex_t reegex;
    int value;
    value = regcomp(&reegex, "[:word:]", 0);
    value = regexec(&reegex, line, 0, NULL, 0);

    if (value == REGEX_SUCCESSFUL) {
        printf("%s", line);
    }
}