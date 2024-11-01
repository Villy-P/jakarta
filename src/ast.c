#include "ast.h"

#include <stdlib.h>

ASTNode* createASTNode(ASTIdentifier identifier, Token* token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->identifier = identifier;
    node->token = token;
    node->nodes_length = 0;
    node->nodes_capacity = 2;
    node->nodes = (ASTNode**)malloc(node->nodes_capacity * sizeof(ASTNode*));
    return node;
}