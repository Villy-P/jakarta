#include <string.h>
#include <stdlib.h>

#include "identifier.h"

Identifier* create_identifier(IdentifierType type, char* name, char* allias, unsigned char size) {
    Identifier* identifier = malloc(sizeof(Identifier));
    identifier->type = type;
    identifier->name = malloc(strlen(name) + 1);
    identifier->allias = malloc(strlen(allias) + 1);
    strcpy(identifier->name, name);
    strcpy(identifier->allias, allias);
    identifier->size = size;
    return identifier;
}