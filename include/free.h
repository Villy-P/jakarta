#ifndef __JAKARTA_FREE_H__
#define __JAKARTA_FREE_H__

#include "data_structures/tokenizer.h"
#include "token.h"
#include "types/types.h"

void free_tokenizer(Tokenizer* tokenizer);
void free_token(Token* token);
void free_type(Type* type);

#endif