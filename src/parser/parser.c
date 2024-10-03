#include "parser.h"
#include "debug.h"

void parse(Tokenizer* tokenizer) {
    debug_message("Begun Parsing", TOP_LEVEL);
    while (tokenizer->current_token_length > 0) {
        if (peek(tokenizer, KEYWORD_TYPEDEF))
            printf("Typedef Found\n");
        consume(tokenizer);
    }
}