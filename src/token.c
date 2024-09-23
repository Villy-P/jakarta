#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "token.h"

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content) {
    Token* item = malloc(sizeof(Token));
    item->symbol = symbol;
    item->line = line;
    item->col = col;
    item->content = malloc(strlen(content) + 1);
    strcpy(item->content, content);
    return item;
}
