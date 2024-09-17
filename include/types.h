#ifndef __JAKARTA_TYPES_H__
#define __JAKARTA_TYPES_H__

#include "identifier.h"

#define INITIAL_TYPES_LENGTH 64

extern Identifier** types;
extern unsigned int max_type_length;
extern unsigned int current_type_length;

void create_base_types();
void add_type(Identifier* type);

#endif