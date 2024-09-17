#ifndef __JAKARTA_IDENTIFIER_H__
#define __JAKARTA_IDENTIFIER_H__

typedef enum IdentifierTypeDef {
    TYPE,
    VARIABLE
} IdentifierType;

typedef struct IdentifierDef {
    IdentifierType type;
    char* name;
    char* allias;
    unsigned char size;
} Identifier;

Identifier* create_identifier(IdentifierType type, char* name, char* allias, unsigned char size);

#endif