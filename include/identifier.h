#ifndef __JAKARTA_IDENTIFIER_H__
#define __JAKARTA_IDENTIFIER_H__

typedef enum IdentifierTypeDef {
    TYPE,
    VARIABLE
} IdentifierType;

typedef struct IdentifierDef {
    IdentifierType type;
    char* name;
    char* content;
    unsigned char size;
} Identifier;

Identifier* create_identifier(Identifier type, char* name, char* content, unsigned char size);

#endif