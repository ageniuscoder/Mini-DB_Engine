#include "parser.h"

unordered_map<TOKEN_SET, NODE_SET> REL_SET = {
    {TOKEN_EQUALS, NODE_CONDITION_EQUALS},
    {TOKEN_NOT_EQUALS, NODE_CONDITION_NOT_EQUALS},
    {TOKEN_LESS_THAN, NODE_CONDITION_LESS_THAN},
    {TOKEN_LESS_THAN_EQUALS, NODE_CONDITION_LESS_THAN_EQUALS},
    {TOKEN_GREATER_THAN, NODE_CONDITION_GREATER_THAN},
    {TOKEN_GREATER_THAN_EQUALS, NODE_CONDITION_GREATER_THAN_EQUALS},
};

unordered_map<TOKEN_SET, string> LOG_SET = {
    {TOKEN_AND, "&&"},
    {TOKEN_OR, "||"},
};

string nodeTypeToString(NODE_SET REQUIRED_NODE)
{
    switch (REQUIRED_NODE)
    {
    case NODE_NEW:
        return "NODE_NEW";
    case NODE_ADD:
        return "NODE_ADD";
    case NODE_PRINT:
        return "NODE_PRINT";
    case NODE_UPDATE:
        return "NODE_UPDATE";
    case NODE_REMOVE:
        return "NODE_REMOVE";
    case NODE_EXIT:
        return "NODE_EXIT";
    case NODE_NOT:
        return "NODE_NOT";
    case NODE_AND:
        return "NODE_AND";
    case NODE_OR:
        return "NODE_OR";
    case NODE_CONDITION:
        return "NODE_CONDITION";
    case NODE_CONDITION_EQUALS:
        return "NODE_CONDITION_EQUALS";
    case NODE_CONDITION_NOT_EQUALS:
        return "NODE_CONDITION_NOT_EQUALS";
    case NODE_CONDITION_GREATER_THAN:
        return "NODE_CONDITION_GREATER_THAN";
    case NODE_CONDITION_LESS_THAN:
        return "NODE_CONDITION_LESS_THAN";
    case NODE_CONDITION_LESS_THAN_EQUALS:
        return "NODE_CONDITION_LESS_THAN_EQUALS";
    case NODE_CONDITION_GREATER_THAN_EQUALS:
        return "NODE_CONDITION_GREATER_THAN_EQUALS";
    case NODE_SUB_VALUES:
        return "NODE_SUB_VALUES";
    case NODE_STRING:
        return "NODE_STRING";
    case NODE_INT:
        return "NODE_INT";
    case NODE_FLOAT:
        return "NODE_FLOAT";
    case NODE_SERVER_CREATE:
        return "NODE_SERVER_CREATE";
    case NODE_SERVER_CONNECT:
        return "NODE_SERVER_CONNECT";
    case NODE_CREATE_DATABASE:
        return "NODE_CREATE_DATABASE";
    case NODE_USE_DATABASE:
        return "NODE_USE_DATABASE";
    case NODE_EXPORT:
        return "NODE_EXPORT";
    }
    return "[!] UNINDENTIFIED NODE : " + REQUIRED_NODE;
}

Parser::Parser() {}

PARSER_STATUS Parser::throwVerboseSyntaxError(TOKEN_SET REQUIRED_TOKEN)
{
    if (error_level == 1)
        return PARSER_FAIL;

    cout << FAIL << "\n[!] SYNTAX ERROR : Unexpected Token : " << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << " , Expected Token : " << tokenTypeToString(REQUIRED_TOKEN) << DEFAULT << endl;
    int positionCounter = 0;
    int arrow_counter = 0;

    for (char currentCharacter : InputBuffer)
    {
        if (positionCounter >= CURRENT_TOKEN->position && positionCounter <= CURRENT_TOKEN->position + CURRENT_TOKEN->VALUE.length())
        {
            if (arrow_counter == 0)
                arrow_counter = positionCounter;
            cout << FAIL << currentCharacter << DEFAULT;
        }
        else
            cout << currentCharacter;
        positionCounter++;
    }
    cout << endl;

    while (arrow_counter--)
        cout << " ";
    cout << FAIL << "^" << DEFAULT;

    cout << "\n\n";
    error_level = 1;
    return PARSER_FAIL;
    return PARSER_FAIL;
}

PARSER_STATUS Parser::throwSyntaxError()
{
    if (error_level == 1)
        return PARSER_FAIL;

    cout << FAIL << "\n[!] SYNTAX ERROR : UNEXPECTED TOKEN : " << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << DEFAULT << endl;
    int positionCounter = 0;
    int arrow_counter = 0;

    for (char currentCharacter : InputBuffer)
    {
        if (positionCounter >= CURRENT_TOKEN->position && positionCounter <= CURRENT_TOKEN->position + CURRENT_TOKEN->VALUE.length())
        {
            if (arrow_counter == 0)
                arrow_counter = positionCounter;
            cout << FAIL << currentCharacter << DEFAULT;
        }
        else
            cout << currentCharacter;
        positionCounter++;
    }
    cout << endl;

    while (arrow_counter--)
        cout << " ";
    cout << FAIL << "^" << DEFAULT;

    cout << "\n\n";
    error_level = 1;
    return PARSER_FAIL;
}

void Parser::check(TOKEN_SET REQUIRED_CHECK_TOKEN)
{
    if (CURRENT_TOKEN->TOKEN_TYPE != REQUIRED_CHECK_TOKEN)
        throwSyntaxError();
}

TOKEN *Parser::proceed(TOKEN_SET REQUIRED_TOKEN)
{
    if (CURRENT_TOKEN->TOKEN_TYPE != REQUIRED_TOKEN)
    {
        throwVerboseSyntaxError(REQUIRED_TOKEN);
        syntaxError = true; // this
        return CURRENT_TOKEN;
    }
    token_number++;
    CURRENT_TOKEN = LOCAL_COPY_TOKEN_STREAM[token_number];
    return CURRENT_TOKEN;
}

TOKEN *Parser::checkAndProceed(TOKEN_SET REQUIRED_TOKEN)
{
    TOKEN *bufferPointer = CURRENT_TOKEN;
    proceed(REQUIRED_TOKEN);
    return bufferPointer;
}

AST_NODE *Parser::parseCONDITION()
{
    AST_NODE *CONDITION_NODE = new AST_NODE;
    CONDITION_NODE->NODE_TYPE = NODE_CONDITION;

    AST_NODE *buffer_pointer;
    while (true)
    {
        string *construct = new string();
        buffer_pointer = new AST_NODE;
        *construct += checkAndProceed(TOKEN_ID)->VALUE;
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DOT)
        {
            proceed(TOKEN_DOT);
            *construct += "." + checkAndProceed(TOKEN_ID)->VALUE;
        }
        buffer_pointer->PAYLOAD = construct;

        if (REL_SET.find(CURRENT_TOKEN->TOKEN_TYPE) == REL_SET.end())
            throwSyntaxError();
        buffer_pointer->NODE_TYPE = REL_SET[CURRENT_TOKEN->TOKEN_TYPE];
        proceed(CURRENT_TOKEN->TOKEN_TYPE);
        switch (CURRENT_TOKEN->TOKEN_TYPE)
        {
        case TOKEN_ID:
        {
            string *construct = new string();
            buffer_pointer->HELPER_TOKEN = TOKEN_ID;
            *construct += checkAndProceed(TOKEN_ID)->VALUE;
            if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DOT)
            {
                proceed(TOKEN_DOT);
                *construct += "." + checkAndProceed(TOKEN_ID)->VALUE;
            }
            buffer_pointer->SUB_PAYLOAD = construct;
            break;
        }
        case TOKEN_INT_DATA:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_INT_DATA;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_INT_DATA)->VALUE;
            break;
        }
        case TOKEN_FLOAT_DATA:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_FLOAT_DATA;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_FLOAT_DATA)->VALUE;
            break;
        }
        case TOKEN_STRING_DATA:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_STRING_DATA;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_STRING_DATA)->VALUE;
            break;
        }
        default:
            throwSyntaxError();
        }
        CONDITION_NODE->CHILDREN.push_back(buffer_pointer);
        if (LOG_SET.find(CURRENT_TOKEN->TOKEN_TYPE) == LOG_SET.end())
            break;
        else
        {
            CONDITION_NODE->DATA_LIST.push_back(LOG_SET[CURRENT_TOKEN->TOKEN_TYPE]);
            proceed(CURRENT_TOKEN->TOKEN_TYPE);
        }
    }

    return CONDITION_NODE;
}

PARSER_STATUS Parser::parseNEW()
{
    /*
    new <table> :: <type> <name> , <type> <name>
    <table> would go in the ->payload
    <columns> would go in the ->children
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_NEW;

    proceed(TOKEN_NEW);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
    proceed(TOKEN_DOUBLE_COLON);

    AST_NODE *buffer_pointer;
    bool primary_key_allocated = false;
    while (true)
    {
        buffer_pointer = new AST_NODE;

        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_PRIMARY)
        {
            if (primary_key_allocated)
            {
                cout << "[!] Error , Cannot have multiple primary keys in a table ! " << endl;
                exit(0);
            }
            primary_key_allocated = true;
            buffer_pointer->isPrimary = true;
            proceed(TOKEN_PRIMARY);
        }

        switch (CURRENT_TOKEN->TOKEN_TYPE)
        {
        case TOKEN_INT:
        {
            proceed(TOKEN_INT);
            buffer_pointer->NODE_TYPE = NODE_INT;
            buffer_pointer->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
            break;
        }
        case TOKEN_STRING:
        {
            proceed(TOKEN_STRING);
            buffer_pointer->NODE_TYPE = NODE_STRING;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_INT_DATA)->VALUE;
            buffer_pointer->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
            break;
        }
        case TOKEN_FLOAT:
        {
            proceed(TOKEN_FLOAT);
            buffer_pointer->NODE_TYPE = NODE_FLOAT;
            buffer_pointer->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
            break;
        }
        default:
            return throwSyntaxError();
        }
        EVALUATED_NODE->CHILDREN.push_back(buffer_pointer);

        if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_COMMA)
            break;
        else
            proceed(TOKEN_COMMA);
    }
    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseADD()
{
    /*
    add <table_name> :: <row1> , <row2> , .... , <rowN>
    <rowN> : [<value1> , <value2>]
    */

    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_ADD;
    proceed(TOKEN_ADD);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;
    proceed(TOKEN_DOUBLE_COLON);
    while (true)
    {
        vector<AST_NODE *> buffer_vector;
        proceed(TOKEN_LEFT_SQR_BRACKET);
        while (true)
        {
            AST_NODE *buffer_ast_node;
            buffer_ast_node = new AST_NODE;
            switch (CURRENT_TOKEN->TOKEN_TYPE)
            {
            case TOKEN_INT_DATA:
            {
                buffer_ast_node->NODE_TYPE = NODE_INT;
                buffer_ast_node->PAYLOAD = &checkAndProceed(TOKEN_INT_DATA)->VALUE;
                break;
            }
            case TOKEN_FLOAT_DATA:
            {
                buffer_ast_node->NODE_TYPE = NODE_FLOAT;
                buffer_ast_node->PAYLOAD = &checkAndProceed(TOKEN_FLOAT_DATA)->VALUE;
                break;
            }
            case TOKEN_STRING_DATA:
            {
                buffer_ast_node->NODE_TYPE = NODE_STRING;
                buffer_ast_node->PAYLOAD = &checkAndProceed(TOKEN_STRING_DATA)->VALUE;
                break;
            }
            default:
                return throwSyntaxError();
            }
            buffer_vector.push_back(buffer_ast_node);
            if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_COMMA)
                break;
            else
                proceed(TOKEN_COMMA);
        }
        proceed(TOKEN_RIGHT_SQR_BRACKET);
        EVALUATED_NODE->MULTI_DATA.push_back(buffer_vector);
        if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_COMMA)
            break;
        else
            proceed(TOKEN_COMMA);
    }
    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parsePRINT()
{
    // print students :: name = "aryan" || id = 9
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_PRINT;
    proceed(TOKEN_PRINT);

    while (true)
    {
        string construct = checkAndProceed(TOKEN_ID)->VALUE;
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DOT)
        {
            proceed(TOKEN_DOT);
            construct += "." + checkAndProceed(TOKEN_ID)->VALUE;
        }
        EVALUATED_NODE->DATA_LIST.push_back(construct);

        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_COMMA)
        {
            proceed(TOKEN_COMMA);
            continue;
        }
        else
            break;
    }
    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
    case TOKEN_END_OF_INPUT:
        return PARSER_SUCCESS;
    case TOKEN_DOUBLE_COLON:
    {
        proceed(TOKEN_DOUBLE_COLON);
        EVALUATED_NODE->CHILD = parseCONDITION();
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;
    }
    default:
        return throwSyntaxError();
    }
}

PARSER_STATUS Parser::parseREMOVE()
{
    /*
    remove students
    remove students :: name = "aryan"
    remove studnets :: students.name = "aryan"
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_REMOVE;
    proceed(TOKEN_REMOVE);

    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
    case TOKEN_END_OF_INPUT:
        return PARSER_SUCCESS;
    case TOKEN_DOUBLE_COLON:
    {
        proceed(TOKEN_DOUBLE_COLON);
        EVALUATED_NODE->CHILD = parseCONDITION();
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;
    }
    default:
        return throwSyntaxError();
    }
}

PARSER_STATUS Parser::parseCREATE()
{
    // create dbname
    // append the name in the bluedb_ds_list file , if not there
    // if there , raise an error
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_CREATE_DATABASE;

    proceed(TOKEN_CREATE);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseUSE()
{
    // use dbname , just set the dbname variable in the execution engine to be following
    // also check that the dbname is in there in the bluedb_dblist file
    // while creating or performing any database activiy , just append the name of the db before it
    // db.table.dat
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_USE_DATABASE;

    proceed(TOKEN_USE);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

void Parser::parseUPDATE_VALUES(AST_NODE *&ROOT_NODE)
{
    // id = <value>
    // <value> : id | string data | int data | float data
    AST_NODE *buffer_pointer;
    while (true)
    {
        buffer_pointer = new AST_NODE;
        string *construct = new string;
        *construct += checkAndProceed(TOKEN_ID)->VALUE;
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DOT)
        {
            proceed(TOKEN_DOT);
            *construct += "." + checkAndProceed(TOKEN_ID)->VALUE;
        }
        buffer_pointer->PAYLOAD = construct;
        proceed(TOKEN_EQUAL_TO);
        switch (CURRENT_TOKEN->TOKEN_TYPE)
        {
        case TOKEN_ID:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_ID;
            string *construct = new string;
            *construct += checkAndProceed(TOKEN_ID)->VALUE;
            if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_DOT)
            {
                proceed(TOKEN_DOT);
                *construct += "." + checkAndProceed(TOKEN_ID)->VALUE;
            }
            buffer_pointer->SUB_PAYLOAD = construct;
            break;
        }
        case TOKEN_INT_DATA:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_INT_DATA;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_INT_DATA)->VALUE;
            break;
        }
        case TOKEN_FLOAT_DATA:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_FLOAT_DATA;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_FLOAT_DATA)->VALUE;
            break;
        }
        case TOKEN_STRING_DATA:
        {
            buffer_pointer->HELPER_TOKEN = TOKEN_STRING_DATA;
            buffer_pointer->SUB_PAYLOAD = &checkAndProceed(TOKEN_STRING_DATA)->VALUE;
            break;
        }
        default:
            throwSyntaxError();
        }
        ROOT_NODE->CHILDREN.push_back(buffer_pointer);
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_COMMA)
            proceed(TOKEN_COMMA);
        else
            break;
    }
}

PARSER_STATUS Parser::parseUPDATE()
{
    /*
    consider the two statements

    1) update students :: name == "aryan" && id == 7 -> name = "aryan kumar"
    this would update all the records-> name to aryan kumar , where the name
    is aryan and id = 7

    2) updating all the element in the table :
    update students -> name = "aryan kumar"
    this would unconditionally update all the elements in the table

    update students :: name == "aryan" || id < 5 -> name = "aryan kumar" , id = 8
    update students -> name = "aryan kumar"

    the name of the table would in the ->payload
    the condition would be int the child node
    the update values would in the children vector
    */

    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_UPDATE;
    proceed(TOKEN_UPDATE);
    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
    case TOKEN_DOUBLE_COLON:
    {
        proceed(TOKEN_DOUBLE_COLON);
        EVALUATED_NODE->CHILD = parseCONDITION();
        proceed(TOKEN_ARROW);
        parseUPDATE_VALUES(EVALUATED_NODE);
        break;
    }
    case TOKEN_ARROW:
    {
        proceed(TOKEN_ARROW);
        parseUPDATE_VALUES(EVALUATED_NODE);
        break;
    }
    default:
        return throwSyntaxError();
    }
    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseSERVER()
{
    EVALUATED_NODE = new AST_NODE;
    proceed(TOKEN_SERVER);

    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
    case TOKEN_SERVER_CONNECT:
    {
        EVALUATED_NODE->NODE_TYPE = NODE_SERVER_CONNECT;
        proceed(TOKEN_SERVER_CONNECT);
        break;
    }
    case TOKEN_SERVER_CREATE:
    {
        EVALUATED_NODE->NODE_TYPE = NODE_SERVER_CREATE;
        proceed(TOKEN_SERVER_CREATE);
        break;
    }
    default:
        return throwSyntaxError();
    }

    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_FLOAT_DATA)->VALUE;
    proceed(TOKEN_COLON);
    EVALUATED_NODE->SUB_PAYLOAD = &checkAndProceed(TOKEN_INT_DATA)->VALUE;
    check(TOKEN_END_OF_INPUT);

    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseEXPORT()
{

    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_EXPORT;
    proceed(TOKEN_EXPORT);

    EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID)->VALUE;

    proceed(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

PARSER_STATUS Parser::parseEXIT()
{
    /*
    exit
    */
    EVALUATED_NODE = new AST_NODE;
    EVALUATED_NODE->NODE_TYPE = NODE_EXIT;
    proceed(TOKEN_EXIT);
    check(TOKEN_END_OF_INPUT);
    return PARSER_SUCCESS;
}

void Parser::initialize(vector<TOKEN *> *TOKEN_LIST_ADDRESS, string inputBuffer)
{
    LOCAL_COPY_TOKEN_STREAM.clear();
    LOCAL_COPY_TOKEN_STREAM = *(TOKEN_LIST_ADDRESS);
    token_number = 0;
    CURRENT_TOKEN = LOCAL_COPY_TOKEN_STREAM[token_number];
    syntaxError = false;
    error_level = 0;
    InputBuffer = inputBuffer;
    cout << "[+] Parser Initialized ! " << InputBuffer << endl;
}

PARSER_STATUS Parser::parse()
{
    switch (CURRENT_TOKEN->TOKEN_TYPE)
    {
    case TOKEN_NEW:
        return parseNEW();
    case TOKEN_ADD:
        return parseADD();
    case TOKEN_PRINT:
        return parsePRINT();
    case TOKEN_REMOVE:
        return parseREMOVE();
    case TOKEN_UPDATE:
        return parseUPDATE();
    case TOKEN_EXIT:
        return parseEXIT();
    case TOKEN_SERVER:
        return parseSERVER();
    case TOKEN_CREATE:
        return parseCREATE();
    case TOKEN_USE:
        return parseUSE();
    case TOKEN_EXPORT:
        return parseEXPORT();
    default:
        return throwSyntaxError();
    }
}