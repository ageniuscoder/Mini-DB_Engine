#include <bits/stdc++.h>
#include "parser.h"
#include "lexer.h"
using namespace std;

#define FAIL "\e[0;31m"
#define SUCCESS "\e[0;32m"
#define DEFAULT "\e[0;37m"

Parser::Parser() {}

PARSER_STATUS Parser::throwSyntaxError()
{
    cout << FAIL << "[!] SYNTAX ERROR : UNEXPECTED TOKEN : " << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << DEFAULT << endl;
    exit(0); // can change behaviour of this
    return PARSER_FAIL;
}

TOKEN *Parser::proceed(TOKEN_SET REQUIRED_TOKEN)
{
    if (CURRENT_TOKEN->TOKEN_TYPE != REQUIRED_TOKEN)
    {
        syntaxError = true;
        return CURRENT_TOKEN;
    }
    token_number++;
    CURRENT_TOKEN = LOCAL_COPY_TOKEN_STREAM[token_number];
    return CURRENT_TOKEN;
}

void Parser::check(TOKEN_SET REQUIRED_CHECK_TOKEN)
{

    if (CURRENT_TOKEN->TOKEN_TYPE != REQUIRED_CHECK_TOKEN)
    {
        throwSyntaxError();
    }
}

TOKEN *Parser::checkAndProceed(TOKEN_SET REQUIRED_TOKEN)
{
    TOKEN *bufferPointer = CURRENT_TOKEN;
    proceed(REQUIRED_TOKEN);
    return bufferPointer;
}

void Parser::intialize(vector<TOKEN *> *TOKEN_LIST_ADDRESS)
{
    LOCAL_COPY_TOKEN_STREAM.clear();
    LOCAL_COPY_TOKEN_STREAM = *(TOKEN_LIST_ADDRESS);
    token_number = 0;
    CURRENT_TOKEN = LOCAL_COPY_TOKEN_STREAM[token_number];
    syntaxError = false;
}

AST_NODE *Parser::parseChildren()
{
    AST_NODE *NEW_CHILD_NODE = new AST_NODE;
    NEW_CHILD_NODE->NODE_TYPE = (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_INTEGER) ? NODE_INTEGER : NODE_STRING;
    NEW_CHILD_NODE->PAYLOAD = &CURRENT_TOKEN->VALUE;

    if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_INTEGER)
    {
        proceed(TOKEN_INTEGER);
    }
    else
    {
        proceed(TOKEN_STRING);
    }

    return NEW_CHILD_NODE;
}

AST_NODE *Parser::parseCondition()
{
    /* SYNTAX OF PARSING CONDITION
       ( ID REL-OP INT / STRING )
       (ENROLLMENT_NUMBER == 56)
       (MARKS < 60)
       (EMPLOYEE_NAME == "LAKSHAY")
       We will put ID in thr PAYLOAD
       We will put the INT / STRING int the SUBPAYLOAD
    */

    proceed(TOKEN_LEFT_PAREN);
    AST_NODE *CONDITION_NODE = new AST_NODE;

    CONDITION_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
    case TOKEN_LESS_THAN:
    {
        CONDITION_NODE->NODE_TYPE = NODE_CONDITION_LESS_THAN;
        proceed(TOKEN_LESS_THAN);
        break;
    }
    case TOKEN_GREATER_THAN:
    {
        CONDITION_NODE->NODE_TYPE = NODE_CONDITION_GREATER_THAN;
        proceed(TOKEN_GREATER_THAN);
        break;
    }
    case TOKEN_EQUALS:
    {
        CONDITION_NODE->NODE_TYPE = NODE_CONDITION_EQUALS;
        proceed(TOKEN_EQUALS);
        break;
    }
    default:
        throwSyntaxError();
    }

    if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_INTEGER)
    {
        CONDITION_NODE->SUB_PAYLOAD = &checkAndProceed(TOKEN_INTEGER)->VALUE;
    }
    else if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_STRING)
    {
        CONDITION_NODE->SUB_PAYLOAD = &checkAndProceed(TOKEN_STRING)->VALUE;
    }
    else
    {
        throwSyntaxError();
    }

    proceed(TOKEN_RIGHT_PAREN);

    return CONDITION_NODE;
}

PARSER_STATUS Parser::parseCreate()
{
    /* SYNTAX:
       CREATE NEW DATABASE <DB_NAME>
       CREATE NEW TABLE <TB_NAME>
    */
    EVALUATED_NODE = new AST_NODE;

    proceed(TOKEN_CREATE);
    proceed(TOKEN_NEW);

    if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DATABASE || CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DATABASE)
    {
        EVALUATED_NODE->NODE_TYPE = NODE_CREATE_DATABASE;
        proceed(TOKEN_DATABASE);
    }
    else if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_TABLE || CURRENT_TOKEN->TOKEN_TYPE == TOKEN_TABLE)
    {
        EVALUATED_NODE->NODE_TYPE = NODE_CREATE_TABLE;
        proceed(TOKEN_TABLE);
    }
    else
    {
        throwSyntaxError();
    }

    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseUse()
{
    /* SYNTAX:
       USE <DB_NAME>
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_USE;
    proceed(TOKEN_USE);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseInsert()
{

    /* SYNTAX:
       INSERT INTO <TB_NAME> VALUE (...)
    */

    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_INSERT;
    proceed(TOKEN_INSERT);
    proceed(TOKEN_INTO);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
    proceed(TOKEN_VALUE);

    proceed(TOKEN_LEFT_PAREN);
    while (true)
    {
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_END_OF_INPUT)
        {
            throwSyntaxError();
        }
        if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_INTEGER && CURRENT_TOKEN->TOKEN_TYPE != TOKEN_STRING)
        {
            throwSyntaxError();
        }

        EVALUATED_NODE->CHILDREN.push_back(parseChildren());

        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_RIGHT_PAREN)
        {
            proceed(TOKEN_RIGHT_PAREN);
            break;
        }
        proceed(TOKEN_COMMA);
    }

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseSearch()
{
    /* SYNTAX:
       SEARCH IN <T_NAME> WHERE (CONDITION)
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_SEARCH;
    proceed(TOKEN_SEARCH);
    proceed(TOKEN_IN);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
    proceed(TOKEN_WHERE);
    EVALUATED_NODE->CHILDREN.push_back(parseCondition());

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseDelete()
{
    /* SYNTAX:
     DELETE FROM <T_NAME>  WHERE (CONDITION)
     T_NAME WOULD BE IN THE PAYLOAD
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_DELETE;
    proceed(TOKEN_DELETE);
    proceed(TOKEN_FROM);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
    proceed(TOKEN_WHERE);
    EVALUATED_NODE->CHILDREN.push_back(parseCondition());

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseUpdate()
{
    /* SYNTAX:
       UPDATE <TB_NAME> WHERE (<CONDITION>) WITH (V1, V2, ...)
       TO GET THE CONDITION FROM THE NODE_UPDATE, ACCESS :
            CURRENT_NODE->CHILDREN[0];
       <CONDITION> : ( ID REL-OP INT / STRING )
       PARSING CONDITION
       THE VALUES (V1, V2, .. Vn) WILL BE PUSHED INTO THE UPDATE_CHILDREN VECTOR
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_UPDATE;
    proceed(TOKEN_UPDATE);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
    proceed(TOKEN_WHERE);
    EVALUATED_NODE->CHILDREN.push_back(parseCondition());
    proceed(TOKEN_WITH);

    proceed(TOKEN_LEFT_PAREN);
    while (true)
    {
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_END_OF_INPUT)
        {
            throwSyntaxError();
        }
        if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_INTEGER && CURRENT_TOKEN->TOKEN_TYPE != TOKEN_STRING)
        {
            throwSyntaxError();
        }

        EVALUATED_NODE->UPDATE_CHILDREN.push_back(parseChildren());

        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_RIGHT_PAREN)
        {
            proceed(TOKEN_RIGHT_PAREN);
            break;
        }
        proceed(TOKEN_COMMA);
    }

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseExit()
{
    /* SYNTAX:
       EXIT
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_EXIT;

    proceed(TOKEN_EXIT);

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parse()
{
    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
        /* For every function, if there is a syntax error inside the function, we
        first need to call the throw syntax error from inside the function, then
        we need to return the parse fail enum. */
    case TOKEN_CREATE:
        return parseCreate();
    case TOKEN_USE:
        return parseUse();
    case TOKEN_INSERT:
        return parseInsert();
    case TOKEN_SEARCH:
        return parseSearch();
    case TOKEN_DELETE:
        return parseDelete();
    case TOKEN_UPDATE:
        return parseUpdate();
    case TOKEN_EXIT:
        return parseExit();
    default:
        return throwSyntaxError();
    }
    return PARSER_SUCCESS;
}