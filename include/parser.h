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

void read_line(char* line);

void get_string(char* line);

#endif