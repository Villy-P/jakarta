#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

#include <stdbool.h>

#include "identifier.h"
#include "tokenizer.h"

void create_base_types(Tokenizer* tokenizer);
Type* create_type(const char* name, unsigned char bit_size, TypeOptions option);

#endif