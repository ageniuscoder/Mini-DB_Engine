#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.cpp"
typedef enum
{
    PARSER_SUCCESS,
    PARSER_FAIL
} PARSER_STATUS;

typedef enum
{
    NODE_NEW,
    NODE_ADD,
    NODE_PRINT,
    NODE_UPDATE,
    NODE_REMOVE,
    NODE_EXIT,
    NODE_NOT,
    NODE_AND,
    NODE_OR,
    NODE_CONDITION,
    NODE_CONDITION_EQUALS,
    NODE_CONDITION_NOT_EQUALS,
    NODE_CONDITION_GREATER_THAN,
    NODE_CONDITION_LESS_THAN,
    NODE_CONDITION_LESS_THAN_EQUALS,
    NODE_CONDITION_GREATER_THAN_EQUALS,
    NODE_SUB_VALUES,
    NODE_STRING,
    NODE_INT,
    NODE_FLOAT,
    NODE_SERVER_CREATE,
    NODE_SERVER_CONNECT,
    NODE_CREATE_DATABASE,
    NODE_USE_DATABASE,
    NODE_EXPORT
} NODE_SET;

struct AST_NODE
{
    NODE_SET NODE_TYPE;
    string *PAYLOAD;
    string *SUB_PAYLOAD;
    vector<string> DATA_LIST;
    AST_NODE *CHILD;
    vector<AST_NODE *> CHILDREN;
    vector<vector<AST_NODE *>> MULTI_DATA;
    TOKEN_SET HELPER_TOKEN;
    bool isPrimary;
    AST_NODE()
    {
        CHILD = nullptr;
        isPrimary = false;
    }
};

extern string nodeTypeToString(NODE_SET REQUIRED_NODE);
extern unordered_map<TOKEN_SET, NODE_SET> REL_SET;
extern unordered_map<TOKEN_SET, string> LOG_SET;

class Parser
{
private:
    TOKEN *CURRENT_TOKEN;
    vector<TOKEN *> LOCAL_COPY_TOKEN_STREAM;
    int token_number;
    bool syntaxError;
    string InputBuffer;

    PARSER_STATUS throwVerboseSyntaxError(TOKEN_SET);
    PARSER_STATUS throwSyntaxError();

    void check(TOKEN_SET REQUIRED_CHECK_TOKEN);
    TOKEN *proceed(TOKEN_SET);
    TOKEN *checkAndProceed(TOKEN_SET);

    AST_NODE *parseCONDITION();

    PARSER_STATUS parseNEW();
    PARSER_STATUS parseADD();
    PARSER_STATUS parsePRINT();
    PARSER_STATUS parseREMOVE();
    PARSER_STATUS parseCREATE();
    PARSER_STATUS parseUSE();
    void parseUPDATE_VALUES(AST_NODE *&ROOT_NODE);
    PARSER_STATUS parseUPDATE();
    PARSER_STATUS parseSERVER();
    PARSER_STATUS parseEXPORT();
    PARSER_STATUS parseEXIT();

public:
    Parser();
    AST_NODE *EVALUATED_NODE;
    int error_level;
    void initialize(vector<TOKEN *> *TOKEN_LIST_ADDRESS, string inputBuffer);
    PARSER_STATUS parse();
};

#endif