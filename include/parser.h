#ifndef __JAKARTA_PARSER_H__
#define __JAKARTA_PARSER_H__

typedef enum SymbolDef {
    SYMBOL_STRING,
    SYMBOL_BACKTICK,
    SYMBOL_TILDE,
    SYMBOL_EXCLAMATION,
    SYMBOL_AT,
    SYMBOL_HASHTAG,
    SYMBOL_DOLLAR,
    SYMBOL_PERCENT,
    SYMBOL_CARROT,
    SYMBOL_AMPERSAND,
    SYMBOL_ASTERISK,
    SYMBOL_OPEN_PARENTHESIS,
    SYMBOL_CLOSE_PARENTHESIS,
    SYMBOL_UNDERSCORE,
    SYMBOL_DASH,
    SYMBOL_PLUS,
    SYMBOL_EQUALS,
    SYMBOL_OPEN_BRACE,
    SYMBOL_CLOSE_BRACE,
    SYMBOL_OPEN_BRACKET,
    SYMBOL_CLOSE_BRACKET,
    SYMBOL_VERTICAL_LINE,
    SYMBOL_BACKSLASH,
    SYMBOL_COLON,
    SYMBOL_SEMICOLON,
    SYMBOL_QUOTATION,
    SYMBOL_APOSTRAPHE,
    SYMBOL_OPEN_ANGLE,
    SYMBOL_CLOSE_ANGLE,
    SYMBOL_COMMA,
    SYMBOL_PERIOD,
    SYMBOL_QUESTION,
    SYMBOL_SLASH,
    SYMBOL_NEWLINE,
    SYMBOL_SPACE
} Symbol;

typedef struct TokenDef {
    Symbol symbol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

extern Token** tokens;
extern unsigned int max_token_length;
extern unsigned int current_token_length;

void read_line(char* line);

char* get_string(char** line);

Token* create_token(Symbol symbol, unsigned int line, unsigned int col, char* content);
Symbol get_symbol_from_char(char ch);

void add_token(Token* token);

#endif