#include <string.h>
#include <stdlib.h>

#include "syntax.h"

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content, char* file_name) {
    Token* item = malloc(sizeof(Token));
    item->symbol = symbol;
    item->line = line;
    item->col = col;
    item->content = malloc(strlen(content) + 1);
    strcpy(item->content, content);
    item->file_name = malloc(strlen(file_name) + 1);
    strcpy(item->file_name, file_name);
    return item;
}
