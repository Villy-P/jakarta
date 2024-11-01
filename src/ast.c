#include "ast.h"

#include <stdlib.h>

ASTNode* createASTNode(ASTIdentifier identifier, Token* token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->identifier = identifier;
    node->token = token;
    node->nodes = NULL;
    node->nodes_length = 0;
    node->nodes_capacity = 0;
    return node;
}