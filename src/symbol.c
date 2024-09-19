#include "symbol.h"
#include "error.h"

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

Symbol get_keyword_from_str(char* str) {
    if (strcmp(str, "as"))
        return KEYWORD_AS;
    if (strcmp(str, "assert"))
        return KEYWORD_ASSERT;
    if (strcmp(str, "break"))
        return KEYWORD_BREAK;
    if (strcmp(str, "case"))
        return KEYWORD_CASE;
    if (strcmp(str, "catch"))
        return KEYWORD_CATCH;
    if (strcmp(str, "class"))
        return KEYWORD_CLASS;
    if (strcmp(str, "constant"))
        return KEYWORD_CONSTANT;
    if (strcmp(str, "do"))
        return KEYWORD_DO;
    if (strcmp(str, "else"))
        return KEYWORD_ELSE;
    if (strcmp(str, "enum"))
        return KEYWORD_ENUM;
    if (strcmp(str, "extends"))
        return KEYWORD_EXTENDS;
    if (strcmp(str, "finally"))
        return KEYWORD_FINALLY;
    if (strcmp(str, "for"))
        return KEYWORD_FOR;
    if (strcmp(str, "from"))
        return KEYWORD_FROM;
    if (strcmp(str, "func"))
        return KEYWORD_FUNC;
    if (strcmp(str, "goto"))
        return KEYWORD_GOTO;
    if (strcmp(str, "if"))
        return KEYWORD_IF;
    if (strcmp(str, "interface"))
        return KEYWORD_INTERFACE;
    if (strcmp(str, "import"))
        return KEYWORD_IMPORT;
    if (strcmp(str, "new"))
        return KEYWORD_NEW;
    if (strcmp(str, "priv"))
        return KEYWORD_PRIV;
    if (strcmp(str, "prot"))
        return KEYWORD_PROT;
    if (strcmp(str, "pub"))
        return KEYWORD_PUB;
    if (strcmp(str, "ret"))
        return KEYWORD_RET;
    if (strcmp(str, "static"))
        return KEYWORD_STATIC;
    if (strcmp(str, "switch"))
        return KEYWORD_SWITCH;
    if (strcmp(str, "this"))
        return KEYWORD_THIS;
    if (strcmp(str, "throws"))
        return KEYWORD_THROWS;
    if (strcmp(str, "try"))
        return KEYWORD_TRY;
    if (strcmp(str, "typedef"))
        return KEYWORD_TYPEDEF;
    if (strcmp(str, "while"))
        return KEYWORD_WHILE;
}
