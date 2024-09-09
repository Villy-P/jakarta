#ifndef __JAKARTA_ERROR_H__
#define __JAKARTA_ERROR_H__

typedef enum {
    indentation,
    comma,
    period,
    quotation,
    apostrophe,
    function,
    open_parenthesis,
    close_parenthesis
} Symbol;

typedef struct {
    Symbol sybmol;
    unsigned int line;
    unsigned int col;
    char* content;
} Token;

extern Token** tokens;
extern unsigned int token_length;

void read_line(char* line);

char* get_string(char** line);

#endif