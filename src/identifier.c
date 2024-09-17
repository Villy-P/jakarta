#include <string.h>
#include <stdlib.h>

#include "identifier.h"

Identifier* create_identifier(IdentifierType type, char* name, char* content, unsigned char size) {
    Identifier* identifier = malloc(sizeof(Identifier));
    identifier->type = type;
    identifier->name = malloc(strlen(name) + 1);
    identifier->content = malloc(strlen(content) + 1);
    strcpy(identifier->name, name);
    strcpy(identifier->content, content);
    identifier->size = size;
    return identifier;
}