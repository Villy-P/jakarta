#pragma once

#include "data_structures/tokenizer.h"
#include "types/types.h"
#include "token.h"

void free_tokenizer(Tokenizer* tokenizer);
void free_token(Token* token);
void free_type(Type* type);