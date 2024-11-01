#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

ASTNode* createASTNode(ASTIdentifier identifier, Token* token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->identifier = identifier;
    node->token = token;
    node->nodes_length = 0;
    node->nodes_capacity = 2;
    node->nodes = (ASTNode**)malloc(node->nodes_capacity * sizeof(ASTNode*));
    return node;
}

void addASTNode(ASTNode* parent, ASTNode* child) {
    if (parent->nodes_length >= parent->nodes_capacity) {
        parent->nodes_capacity *= 2;
        parent->nodes = (ASTNode**)realloc(parent->nodes, parent->nodes_capacity * sizeof(ASTNode*));
    }
    parent->nodes[parent->nodes_length++] = child;
}

void printASTNode(ASTNode* node, int depth) {
    if (node == NULL) 
        return;
    for (int i = 0; i < depth; ++i)
        printf("  ");
    printf("Node Identifier: %d\n", node->identifier);
    for (int i = 0; i < node->nodes_length; ++i)
        printASTNode(node->nodes[i], depth + 1);
}