#pragma once
#ifndef __PARSER_H
#define __PARSER_H
#include "lexer.h"

typedef enum
{
    PARSER_FAIL,
    PARSER_SUCCESS,
} PARSER_STATUS;

typedef enum
{
    NODE_CREATE_DATABASE,
    NODE_CREATE_TABLE,
    NODE_USE,
    NODE_INSERT,
    NODE_SEARCH,
    NODE_DELETE,
    NODE_UPDATE,
    NODE_STRING,
    NODE_INTEGER,
    NODE_SUB_VALUES,
    NODE_CONDITION_EQUALS,
    NODE_CONDITION_GREATER_THAN,
    NODE_CONDITION_LESS_THAN,
    NODE_EXIT,
} NODE_SET;

struct AST_NODE
{
    NODE_SET NODE_TYPE;
    string *PAYLOAD;
    string *SUB_PAYLOAD;
    vector<AST_NODE *> CHILDREN;
    vector<AST_NODE *> UPDATE_CHILDREN;
};

string nodeTypeToString(NODE_SET REQUIRED_NODE)
{
    switch (REQUIRED_NODE)
    {
    case NODE_CREATE_DATABASE:
        return "NODE_CREATE_DATABASE";
    case NODE_CREATE_TABLE:
        return "NODE_CREATE_TABLE";
    case NODE_USE:
        return "NODE_USE";
    case NODE_INSERT:
        return "NODE_INSERT";
    case NODE_SEARCH:
        return "NODE_SEARCH";
    case NODE_DELETE:
        return "NODE_DELETE";
    case NODE_UPDATE:
        return "NODE_UPDATE";
    case NODE_EXIT:
        return "NODE_EXIT";
    case NODE_SUB_VALUES:
        return "NODE_SUB_VALUES";
    case NODE_STRING:
        return "NODE_STRING";
    case NODE_INTEGER:
        return "NODE_INTEGER";
    case NODE_CONDITION_EQUALS:
        return "NODE_CONDITION_EQUALS";
    case NODE_CONDITION_GREATER_THAN:
        return "NODE_CONDITION_GREATER_THAN";
    case NODE_CONDITION_LESS_THAN:
        return "NODE_CONDITION_LESS_THAN";
    }

    return "[!] UNIDENTIFIED NODE : " + REQUIRED_NODE;
}

class Parser
{
private:
    int token_number;
    TOKEN *CURRENT_TOKEN;
    AST_NODE *EVALUATED_NODE;
    vector<TOKEN *> LOCAL_COPY_TOKEN_STREAM;
    bool syntaxError;
    TOKEN *proceed(TOKEN_SET REQUIRED_TOKEN);
    TOKEN* checkAndProceed(TOKEN_SET REQUIRED_TOKEN);
    void check(TOKEN_SET REQUIRED_CHECK_TOKEN);
    PARSER_STATUS throwSyntaxError();
    PARSER_STATUS parseCreate();
    PARSER_STATUS parseUse();
    PARSER_STATUS parseInsert();
    PARSER_STATUS parseSearch();
    PARSER_STATUS parseDelete();
    PARSER_STATUS parseUpdate();
    PARSER_STATUS parseExit();
    AST_NODE *parseChildren();
    AST_NODE *parseCondition();
    AST_NODE *parseString_Integer();

public:
    Parser();
    PARSER_STATUS parse();
    void intialize(vector<TOKEN *> *TOKEN_LIST_ADDRESS);
};

#endif