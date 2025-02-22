#include "data_structures/ast.h"

#include <stdlib.h>
#include <stdio.h>

ASTNode* create_ast_node(ASTIdentifier identifier, Token* token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory for ASTNode\n");
        exit(EXIT_FAILURE);
    }
    node->identifier = identifier;
    node->token = token;
    node->nodes = create_array(2);
    return node;
}

AST* create_ast() {
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->root = create_ast_node(AST_IDENTIFIER_BASE_PROGRAM, NULL);
    return ast;
}

void print_ast_node(ASTNode* node, int depth) {
    if (node == NULL) 
        return;
    for (int i = 0; i < depth; ++i)
        printf("  ");
    printf("Node Identifier: %d", node->identifier);
    if (node->token != NULL)
        printf(" Token: %s", node->token->content);
    printf("\n");
    for (int i = 0; i < node->nodes->length; ++i)
        print_ast_node(get_from_array(node->nodes, i), depth + 1);
}