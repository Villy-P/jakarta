#ifndef __JAKARTA_POSTFIX_H__
#define __JAKARTA_POSTFIX_H__

#include "tokenizer.h"
#include "stack.h"

Stack* infix_to_postfix(Tokenizer* tokenizer);
unsigned int precedence(char* op);

#endif