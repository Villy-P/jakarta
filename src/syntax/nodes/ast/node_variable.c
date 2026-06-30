#include <stddef.h>

#include "core.h"
#include "data_structures/ast.h"
#include "data_structures/compiler_state.h"
#include "data_structures/symbol_table.h"
#include "data_structures/tokenizer.h"
#include "debug.h"
#include "semantic_analyzer.h"
#include "syntax.h"

void parse_variable(Tokenizer* tokenizer, ASTNode* ast_node) {
    ASTNode* variable_node = parse_variable_declaration(tokenizer);
    if (peek(tokenizer, SYMBOL_SEMICOLON)) {
        consume(tokenizer);
        ASTNode* variable_content_node = create_ast_node(AST_IDENTIFIER_VARIABLE_CONTENT, nullptr);
        ds_array_push(variable_node->nodes, variable_content_node);
        ds_array_push(ast_node->nodes, variable_node);
        return;
    }
    Token* equal_token = peek_consume(tokenizer, SYMBOL_EQUALS);

    ds_stack* postfix = infix_to_postfix(tokenizer);
    ASTNode* expression = postfix_to_ast(postfix);

    ASTNode* variable_content_node = create_ast_node(AST_IDENTIFIER_VARIABLE_CONTENT, nullptr);

    ds_array_push(variable_content_node->nodes, expression);
    ds_array_push(variable_node->nodes, variable_content_node);
    ds_array_push(ast_node->nodes, variable_node);

    free_token(equal_token);
}

ASTNode* parse_variable_declaration(Tokenizer* tokenizer) {
    Token* type_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);

    if (peek(tokenizer, OPERATOR_ARRAY_DECLARATION)) {
        consume(tokenizer);
    }

    Token* name_token = peek_consume(tokenizer, SYMBOL_IDENTIFIER);

    ASTNode* variable_type_node = create_ast_node(AST_IDENTIFIER_VARIABLE_TYPE, type_token);
    ASTNode* variable_node = create_ast_node(AST_IDENTIFIER_VARIABLE_DEFINITION, name_token);

    ds_array_push(variable_node->nodes, variable_type_node);

    return variable_node;
}






void resolve_variable_definition(ASTNode* node, SymbolTable* symbol_table, CompilerState* state) {
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolving variable definition: %s", node->token->content);

    ASTNode* variable_type_node = DSM_ARRAY_GET(node->nodes, 0, ASTNode*);
    SymbolTableEntry* type_entry = lookup_type(variable_type_node->token->content, symbol_table);
    if (type_entry == nullptr) {
        return handle_error(
            ERROR_UNDEFINED_TYPE, 
            variable_type_node->token, 
            state, 
            variable_type_node->token->content);
    }
    log_msg(logs.main, "[SEMANTIC ANALYZER] Resolved variable type: %s", type_entry->name);

    ASTNode* variable_content_node = DSM_ARRAY_GET(node->nodes, 1, ASTNode*);
    if (variable_content_node->nodes->length == 0) {
        return;
    }
    // ASTNode* expression_node = (ASTNode*)get_from_array(variable_content_node->nodes, 0);
    // TypeRegistryEntry* type = resolve_expression(expression_node, symbol_table, state);
    // TODO(Valerius Petrini): check to see if it matches the type
}