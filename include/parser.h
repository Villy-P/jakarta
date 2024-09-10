#ifndef __JAKARTA_PARSER_H__
#define __JAKARTA_PARSER_H__

typedef enum SymbolDef {
    indentation,
    comma,
    period,
    quotation,
    apostrophe,
    function,
    open_parenthesis,
    close_parenthesis
} Symbol;

typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

extern Token** tokens;
extern unsigned int token_length;

void read_line(char* line);

char* get_string(char** line);

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);

#endif