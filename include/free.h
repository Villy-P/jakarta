#ifndef __JAKARTA_FREE_H__
#define __JAKARTA_FREE_H__

#include "tokenizer.h"
#include "token.h"

void free_tokenizer(Tokenizer* tokenizer);
void free_token(Token* token);

#endif