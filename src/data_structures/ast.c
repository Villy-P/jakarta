#include "data_structures/ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "syntax.h"

static const int INITIAL_ASTNODE_CHILDREN_CAPACITY = 2;
static const int AST_PRINT_BUFFER_SIZE = 512;

const char* const AST_NODE_NAMES[] = {
    [AST_IDENTIFIER_BASE_PROGRAM] = "ROOT",

    [AST_IDENTIFIER_IMPORT_STATEMENT] = "Import Statement",

    [AST_IDENTIFIER_TYPE_DEFINITION] = "Typedef",
    [AST_IDENTIFIER_TYPE_DEFINITION_ALIAS] = "Typedef Alias",
    [AST_IDENTIFIER_TYPE_DEFINITION_TYPE] = "Typedef Type",

    [AST_IDENTIFIER_FUNCTION_DEFINITION] = "Function Definition",
    [AST_IDENTIFIER_FUNCTION_RETURN_TYPE] = "Function Return Type",
    [AST_IDENTIFIER_FUNCTION_PARAMETERS] = "Function Parameters",
    [AST_IDENTIFIER_FUNCTION_PARAMETER] = "Function Parameter",
    [AST_IDENTIFIER_FUNCTION_PARAMETER_TYPE] = "Function Parameter Type",
    [AST_IDENTIFIER_FUNCTION_BODY] = "Function Body",
    [AST_IDENTIFIER_FUNCTION_CALL] = "Function Call",

    [AST_IDENTIFIER_VARIABLE_DEFINITION] = "Variable Definition",
    [AST_IDENTIFIER_VARIABLE_TYPE] = "Variable Type",
    [AST_IDENTIFIER_VARIABLE_CONTENT] = "Variable Content",

    [AST_IDENTIFIER_CLASS_CREATOR] = "Class",
    [AST_IDENTIFIER_CLASS_BODY] = "Class Body",

    [AST_IDENTIFIER_IF_STATEMENT] = "If Statement",
    [AST_IDENTIFIER_IF_CONDITION] = "If Condition",
    [AST_IDENTIFIER_IF_BODY] = "If Body",
    [AST_IDENTIFIER_ELSE_BODY] = "Else Body",
    [AST_IDENTIFIER_ELSE_IF_LIST] = "Else If List",
    [AST_IDENTIFIER_ELSE_IF_STATEMENT] = "Else If Statement",
    [AST_IDENTIFIER_ELSE_IF_BODY] = "Else If Body",

    [AST_IDENTIFIER_INCREMENT] = "Increment",
    [AST_IDENTIFIER_DECREMENT] = "Decrement",

    [AST_IDENTIFIER_OPERATOR] = "Operator",
    [AST_IDENTIFIER_VALUE] = "Value",

    [AST_IDENTIFIER_RETURN] = "Return",

    [AST_IDENTIFIER_FOR] = "For Loop",
    [AST_IDENTIFIER_FOR_INITIALIZATION] = "For Loop Initialization",
    [AST_IDENTIFIER_FOR_CONDITION] = "For Loop Condition",
    [AST_IDENTIFIER_FOR_ITERATION] = "For Loop Iteration",
    [AST_IDENTIFIER_FOR_BODY] = "For Loop Body",

    [AST_IDENTIFIER_ARRAY_ACCESS] = "Array Access",
    [AST_IDENTIFIER_INDEX] = "Array Index",

    [AST_LITERAL] = "Variable",
    [AST_NUMBER] = "Number",
    [AST_OPERATOR] = "Operator",
    [AST_DOT_OPERATOR] = "Dot Operator"};

ASTNode* create_ast_node(ASTIdentifier identifier, Token* token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == nullptr) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "ASTNode");
        return nullptr;
    }
    node->identifier = identifier;
    node->token = token;
    node->nodes = ds_astnode_ptr_array_create(INITIAL_ASTNODE_CHILDREN_CAPACITY,
                                              nullptr, nullptr, nullptr);
    if (node->nodes == nullptr) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "ASTNode children");
        free(node);
        return nullptr;
    }
    return node;
}

void print_ast_node(ASTNode* node, const char* prefix, bool is_last) {
    if (node == nullptr) {
        return;
    }

    const char* node_name = AST_NODE_NAMES[node->identifier];
    if (node_name == nullptr) {
        node_name = "UNKNOWN_NODE";
    }

    const char* marker = (int)is_last ? "└── " : "├── ";
    if (fprintf(logs.ast, "%s%s%s", prefix, marker, node_name) < 0) {
        jakarta_error(ERR_CUSTOM, nullptr, "Failed to print AST node");
    }

    if (node->token != nullptr && node->token->content != nullptr) {
        if (fprintf(logs.ast, ": %s", node->token->content) < 0) {
            jakarta_error(ERR_CUSTOM, nullptr,
                          "Failed to print AST node content");
        }
    }
    if (fprintf(logs.ast, "\n") < 0) {
        jakarta_error(ERR_CUSTOM, nullptr, "Failed to print AST node newline");
    }

    char new_prefix[AST_PRINT_BUFFER_SIZE];
    if (snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix,
                 (int)is_last ? "    " : "│   ") < 0) {
        jakarta_error(ERR_CUSTOM, nullptr, "Failed to create new prefix");
    }

    unsigned int num_children = node->nodes->length;
    for (unsigned int i = 0; i < num_children; ++i) {
        ASTNode* child = ds_astnode_ptr_array_get(node->nodes, i);
        bool child_is_last = (i == num_children - 1);

        print_ast_node(child, new_prefix, child_is_last);
    }
}