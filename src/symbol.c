#include <string.h>
#include <regex.h>

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
        case '"': return SYMBOL_QUOTATION;
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
    jakarta_error(ERR_UNKNOWN_SYMBOL, NULL, "");
    return SYMBOL_NONE;
}

char* get_string_from_symbol(Symbol symbol) {
    switch (symbol) {
        case SYMBOL_BACKTICK:          return "`";
        case SYMBOL_TILDE:             return "~";
        case SYMBOL_EXCLAMATION:       return "!";
        case SYMBOL_AT:                return "@";
        case SYMBOL_HASHTAG:           return "#";
        case SYMBOL_DOLLAR:            return "$";
        case SYMBOL_PERCENT:           return "%";
        case SYMBOL_CARROT:            return "^";
        case SYMBOL_AMPERSAND:         return "&";
        case SYMBOL_ASTERISK:          return "*";
        case SYMBOL_OPEN_PARENTHESIS:  return "(";
        case SYMBOL_CLOSE_PARENTHESIS: return ")";
        case SYMBOL_DASH:              return "-";
        case SYMBOL_UNDERSCORE:        return "_";
        case SYMBOL_PLUS:              return "+";
        case SYMBOL_EQUALS:            return "=";
        case SYMBOL_OPEN_BRACKET:      return "[";
        case SYMBOL_CLOSE_BRACKET:     return "]";
        case SYMBOL_OPEN_BRACE:        return "{";
        case SYMBOL_CLOSE_BRACE:       return "}";
        case SYMBOL_VERTICAL_LINE:     return "|";
        case SYMBOL_COLON:             return ":";
        case SYMBOL_SEMICOLON:         return ";";
        case SYMBOL_OPEN_ANGLE:        return "<";
        case SYMBOL_CLOSE_ANGLE:       return ">";
        case SYMBOL_COMMA:             return ",";
        case SYMBOL_PERIOD:            return ".";
        case SYMBOL_QUESTION:          return "?";
        case SYMBOL_SLASH:             return "/";
        case SYMBOL_SPACE:             return " ";
        case SYMBOL_NEWLINE:           return "\n";
        case SYMBOL_QUOTATION:         return "\"";
        case SYMBOL_BACKSLASH:         return "\\";
        case SYMBOL_APOSTRAPHE:        return "\'";

        case SYMBOL_IDENTIFIER: return "IDENTIFIER";
        case SYMBOL_NUMBER: return "NUMBER";

        case KEYWORD_AS:        return "as";
        case KEYWORD_ASSERT:    return "assert";
        case KEYWORD_BREAK:     return "break";
        case KEYWORD_CASE:      return "case";
        case KEYWORD_CATCH:     return "catch";
        case KEYWORD_CLASS:     return "class";
        case KEYWORD_CONSTANT:  return "constant";
        case KEYWORD_DO:        return "do";
        case KEYWORD_ELSE:      return "else";
        case KEYWORD_ENUM:      return "enum";
        case KEYWORD_EXTENDS:   return "extends";
        case KEYWORD_FINALLY:   return "finally";
        case KEYWORD_FOR:       return "for";
        case KEYWORD_FROM:      return "from";
        case KEYWORD_FUNC:      return "func";
        case KEYWORD_GOTO:      return "goto";
        case KEYWORD_IF:        return "if";
        case KEYWORD_INTERFACE: return "interface";
        case KEYWORD_IMPORT:    return "import";
        case KEYWORD_NEW:       return "new";
        case KEYWORD_PRIV:      return "priv";
        case KEYWORD_PROT:      return "prot";
        case KEYWORD_PUB:       return "pub";
        case KEYWORD_RET:       return "ret";
        case KEYWORD_STATIC:    return "static";
        case KEYWORD_SWITCH:    return "switch";
        case KEYWORD_THIS:      return "this";
        case KEYWORD_THROWS:    return "throws";
        case KEYWORD_TRY:       return "try";
        case KEYWORD_TYPEDEF:   return "typedef";
        case KEYWORD_WHILE:     return "while";
        default: break;
    }
    return "none";
}

Symbol get_keyword_from_str(char* str) {
    if (is_number_symbol(str))         return SYMBOL_NUMBER;
    if (strcmp(str, "as")        == 0) return KEYWORD_AS;
    if (strcmp(str, "assert")    == 0) return KEYWORD_ASSERT;
    if (strcmp(str, "break")     == 0) return KEYWORD_BREAK;
    if (strcmp(str, "case")      == 0) return KEYWORD_CASE;
    if (strcmp(str, "catch")     == 0) return KEYWORD_CATCH;
    if (strcmp(str, "class")     == 0) return KEYWORD_CLASS;
    if (strcmp(str, "constant")  == 0) return KEYWORD_CONSTANT;
    if (strcmp(str, "do")        == 0) return KEYWORD_DO;
    if (strcmp(str, "else")      == 0) return KEYWORD_ELSE;
    if (strcmp(str, "enum")      == 0) return KEYWORD_ENUM;
    if (strcmp(str, "extends")   == 0) return KEYWORD_EXTENDS;
    if (strcmp(str, "finally")   == 0) return KEYWORD_FINALLY;
    if (strcmp(str, "for")       == 0) return KEYWORD_FOR;
    if (strcmp(str, "from")      == 0) return KEYWORD_FROM;
    if (strcmp(str, "func")      == 0) return KEYWORD_FUNC;
    if (strcmp(str, "goto")      == 0) return KEYWORD_GOTO;
    if (strcmp(str, "if")        == 0) return KEYWORD_IF;
    if (strcmp(str, "interface") == 0) return KEYWORD_INTERFACE;
    if (strcmp(str, "import")    == 0) return KEYWORD_IMPORT;
    if (strcmp(str, "new")       == 0) return KEYWORD_NEW;
    if (strcmp(str, "priv")      == 0) return KEYWORD_PRIV;
    if (strcmp(str, "prot")      == 0) return KEYWORD_PROT;
    if (strcmp(str, "pub")       == 0) return KEYWORD_PUB;
    if (strcmp(str, "ret")       == 0) return KEYWORD_RET;
    if (strcmp(str, "static")    == 0) return KEYWORD_STATIC;
    if (strcmp(str, "switch")    == 0) return KEYWORD_SWITCH;
    if (strcmp(str, "this")      == 0) return KEYWORD_THIS;
    if (strcmp(str, "throws")    == 0) return KEYWORD_THROWS;
    if (strcmp(str, "try")       == 0) return KEYWORD_TRY;
    if (strcmp(str, "typedef")   == 0) return KEYWORD_TYPEDEF;
    if (strcmp(str, "while")     == 0) return KEYWORD_WHILE;

    if (strcmp(str, "==") == 0)  return OPERATOR_EQUIVALENCE;
    if (strcmp(str, "!=") == 0)  return OPERATOR_NOT_EQUIVALENCE;
    if (strcmp(str, ">=") == 0)  return OPERATOR_GREATER_THAN_OR_EQUAL_TO;
    if (strcmp(str, "<=") == 0)  return OPERATOR_LESS_THAN_OR_EQUAL_TO;
    if (strcmp(str, "++") == 0)  return OPERATOR_INCREMENT;
    if (strcmp(str, "--") == 0)  return OPERATOR_DECREMENT;
    if (strcmp(str, "+=") == 0)  return OPERATOR_ADDITION_ASSIGNMENT;
    if (strcmp(str, "-=") == 0)  return OPERATOR_SUBTRACTION_ASSIGNMENT;
    if (strcmp(str, "*=") == 0)  return OPERATOR_MULTIPLICATION_ASSIGNMENT;
    if (strcmp(str, "/=") == 0)  return OPERATOR_DIVISION_ASSIGNMENT;
    if (strcmp(str, "%=") == 0)  return OPERATOR_MODULUS_ASSIGNMENT;
    if (strcmp(str, "&=") == 0)  return OPERATOR_BITWISE_AND_ASSIGNMENT;
    if (strcmp(str, "|=") == 0)  return OPERATOR_BITWISE_OR_ASSIGNMENT;
    if (strcmp(str, "^=") == 0)  return OPERATOR_BITWISE_XOR_ASSIGNMENT;
    if (strcmp(str, "<<") == 0)  return OPERATOR_LEFT_SHIFT;
    if (strcmp(str, ">>") == 0)  return OPERATOR_RIGHT_SHIFT;
    if (strcmp(str, ">>>") == 0) return OPERATOR_RIGHT_SHIFT_UNSIGNED;
    if (strcmp(str, "!") == 0)   return OPERATOR_NOT;
    if (strcmp(str, "&") == 0)   return OPERATOR_BITWISE_AND;
    if (strcmp(str, "|") == 0)   return OPERATOR_BITWISE_OR;
    if (strcmp(str, "^") == 0)   return OPERATOR_BITWISE_XOR;
    if (strcmp(str, "~") == 0)   return OPERATOR_BITWISE_NOT;
    if (strcmp(str, "&&") == 0)  return OPERATOR_LOGICAL_AND;
    if (strcmp(str, "||") == 0)  return OPERATOR_LOGICAL_OR;
    return SYMBOL_IDENTIFIER;
}

bool is_number_symbol(char* str) {
    regex_t regex;
    int reti;

    reti = regcomp(&regex, "^-?(0x[A-F0-9]+|0b[0-1]+|0[0-7]+|\\d*(\\.\\d*)?|\\d+e\\d+)$", REG_EXTENDED);

    reti = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);

    return !reti;
}
