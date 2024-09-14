#include "symbol.h"

Symbol get_symbol_from_char(char ch) {
    switch (ch) {
        case '`': return SYMBOL_BACKTICK;
        case '~': return SYMBOL_TILDE;
        case '!': return SYMBOL_EXCLAMATION;
        case '@': return SYMBOL_AT;
        case '#': return SYMBOL_HASHTAG;
        case '$': return SYMBOL_DOLLAR;
        case '%': return SYMBOL_PERCENT;
        case '^': return SYMBOL_CARROT;
        case '&': return SYMBOL_AMPERSAND;
        case '*': return SYMBOL_ASTERISK;
        case '(': return SYMBOL_OPEN_PARENTHESIS;
        case ')': return SYMBOL_CLOSE_PARENTHESIS;
        case '-': return SYMBOL_DASH;
        case '_': return SYMBOL_UNDERSCORE;
        case '+': return SYMBOL_PLUS;
        case '=': return SYMBOL_EQUALS;
        case '[': return SYMBOL_OPEN_BRACKET;
        case ']': return SYMBOL_CLOSE_BRACKET;
        case '{': return SYMBOL_OPEN_BRACE;
        case '}': return SYMBOL_CLOSE_BRACE;
        case '|': return SYMBOL_VERTICAL_LINE;
        case ':': return SYMBOL_COLON;
        case ';': return SYMBOL_SEMICOLON;
        case '"': return SYMBOL_QUESTION;
        case '<': return SYMBOL_OPEN_ANGLE;
        case '>': return SYMBOL_CLOSE_ANGLE;
        case ',': return SYMBOL_COMMA;
        case '.': return SYMBOL_PERIOD;
        case '?': return SYMBOL_QUESTION;
        case '/': return SYMBOL_SLASH;
        case ' ': return SYMBOL_SPACE;
        case '\n': return SYMBOL_NEWLINE;
        case '\\': return SYMBOL_BACKSLASH;
        case '\'': return SYMBOL_APOSTRAPHE;
        default: break;
    };
    jakarta_error_unknown_symbol(ch);
    return SYMBOL_NONE;
}
