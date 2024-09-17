#ifndef __JAKARTA_IDENTIFIER_H__
#define __JAKARTA_IDENTIFIER_H__

typedef enum IdentifierType {
    TYPE,
    VARIABLE
};

typedef struct IdentiferDef {
    IdentifierType type;
    char* name;
    char* content;
    unsigned char size;
} Identifer;

#endif