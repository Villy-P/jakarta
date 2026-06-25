#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "symbol.h"
#include "syntax.h"

Token* create_token(Symbol symbol, uint32_t line, uint32_t col, const char* content, const char* file_name) {
    if (content == nullptr) {
        jakarta_error(ERR_CUSTOM, nullptr, "Token content cannot be null");
        return nullptr;
    }
    if (file_name == nullptr) {
        jakarta_error(ERR_CUSTOM, nullptr, "Token file name cannot be null");
        return nullptr;
    }
    Token* item = malloc(sizeof(Token));
    if (item == nullptr) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Token");
        return nullptr;
    }
    item->symbol = symbol;
    item->line = line;
    item->col = col;
    item->content = strdup(content);
    item->file_name = strdup(file_name);
    return item;
}
