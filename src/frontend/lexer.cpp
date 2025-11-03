#include "lexer.h"

unordered_map<string, TOKEN_SET> KEYWORD_MAP = {
    {"int", TOKEN_INT},
    {"float", TOKEN_FLOAT},
    {"string", TOKEN_STRING},
    {"char", TOKEN_CHAR},
    {"bool", TOKEN_BOOL},
    {"date", TOKEN_DATE},
    {"time", TOKEN_TIME},
    {"new", TOKEN_NEW},
    {"add", TOKEN_ADD},
    {"print", TOKEN_PRINT},
    {"remove", TOKEN_REMOVE},
    {"update", TOKEN_UPDATE},
    {"exit", TOKEN_EXIT},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {"server", TOKEN_SERVER},
    {"socket_create", TOKEN_SERVER_CREATE},
    {"socket_connect", TOKEN_SERVER_CONNECT},
    {"create", TOKEN_CREATE},
    {"use", TOKEN_USE},
    {"primary", TOKEN_PRIMARY},
    {"export", TOKEN_EXPORT},
};

string tokenTypeToString(TOKEN_SET REQUIRED_TOKEN)
{
    switch (REQUIRED_TOKEN)
    {
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_FLOAT:
        return "TOKEN_FLOAT";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_CHAR:
        return "TOKEN_CHAR";
    case TOKEN_BOOL:
        return "TOKEN_BOOL";
    case TOKEN_DATE:
        return "TOKEN_DATE";
    case TOKEN_TIME:
        return "TOKEN_TIME";
    case TOKEN_NEW:
        return "TOKEN_NEW";
    case TOKEN_DOUBLE_COLON:
        return "TOKEN_DOUBLE_COLON";
    case TOKEN_DOT:
        return "TOKEN_DOT";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_ADD:
        return "TOKEN_ADD";
    case TOKEN_LEFT_SQR_BRACKET:
        return "TOKEN_LEFT_SQR_BRACKET";
    case TOKEN_RIGHT_SQR_BRACKET:
        return "TOKEN_RIGHT_SQR_BRACKET";
    case TOKEN_LEFT_PAREN:
        return "TOKEN_LEFT_PAREN";
    case TOKEN_RIGHT_PAREN:
        return "TOKEN_RIGHT_PAREN";
    case TOKEN_STRING_DATA:
        return "TOKEN_STRING_DATA";
    case TOKEN_INT_DATA:
        return "TOKEN_INT_DATA";
    case TOKEN_FLOAT_DATA:
        return "TOKEN_FLOAT_DATA";
    case TOKEN_TRUE:
        return "TOKEN_TRUE";
    case TOKEN_FALSE:
        return "TOKEN_FALSE";
    case TOKEN_CREATE:
        return "TOKEN_CREATE";
    case TOKEN_USE:
        return "TOKEN_USE";
    case TOKEN_PRINT:
        return "TOKEN_PRINT";
    case TOKEN_REMOVE:
        return "TOKEN_REMOVE";
    case TOKEN_UPDATE:
        return "TOKEN_UPDATE";
    case TOKEN_NOT:
        return "TOKEN_NOT";
    case TOKEN_OR:
        return "TOKEN_OR";
    case TOKEN_AND:
        return "TOKEN_AND";
    case TOKEN_ARROW:
        return "TOKEN_ARROW";
    case TOKEN_EQUAL_TO:
        return "TOKEN_EQUAL_TO";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_NOT_EQUALS:
        return "TOKEN_NOT_EQUALS";
    case TOKEN_LESS_THAN:
        return "TOKEN_LESS_THAN";
    case TOKEN_GREATER_THAN:
        return "TOKEN_GREATER_THAN";
    case TOKEN_LESS_THAN_EQUALS:
        return "TOKEN_LESS_THAN_EQUALS";
    case TOKEN_GREATER_THAN_EQUALS:
        return "TOKEN_GREATER_THAN_EQUALS";
    case TOKEN_ID:
        return "TOKEN_ID";
    case TOKEN_EXIT:
        return "TOKEN_EXIT";
    case TOKEN_END_OF_INPUT:
        return "TOKEN_END_OF_INPUT";
    case TOKEN_SERVER:
        return "TOKEN_SERVER";
    case TOKEN_SERVER_CONNECT:
        return "TOKEN_SERVER_CONNECT";
    case TOKEN_SERVER_CREATE:
        return "TOKEN_SERVER_CREATE";
    case TOKEN_COLON:
        return "TOKEN_COLON";
    case TOKEN_PRIMARY:
        return "TOKEN_PRIMARY";
    case TOKEN_EXPORT:
        return "TOKEN_EXPORT";
    }
    return "[!] ERROR : UNIDENTIFIED TOKEN : " + REQUIRED_TOKEN;
}

Lexer::Lexer() {}

char Lexer::advance()
{
    if (cursor == length - 1) // this means that we are at the end of the input buffer
    {
        current = '\0';
        return current;
    }
    else
    {
        current = LocalInputBuffer[++cursor];
        return current;
    }
}

void Lexer::skipWhiteSpaces()
{
    while (current == ' ' && current != '\0')
        advance();
}

TOKEN *Lexer::tokenizeSTRING()
{
    advance(); // advancing the opening quotes
    TOKEN *newToken = new TOKEN;
    newToken->position = cursor;
    string temporaryBuffer = "";
    while (current != '"')
    {
        if (current == '\0')
        {
            stringParsingError = true;
            break;
        }
        temporaryBuffer.push_back(current);
        advance();
    }
    advance(); // advancing the closing quotes

    newToken->TOKEN_TYPE = TOKEN_STRING_DATA;
    newToken->VALUE = temporaryBuffer;
    return newToken;
}

TOKEN *Lexer::tokenizeID()
{
    TOKEN *newToken = new TOKEN;
    newToken->position = cursor;
    string temporaryBuffer = "";

    temporaryBuffer.push_back(current);
    advance();

    while (isalnum(current) || current == '_')
    {
        temporaryBuffer.push_back(current);
        advance();
    }

    newToken->TOKEN_TYPE = TOKEN_ID;
    newToken->VALUE = temporaryBuffer;
    if (KEYWORD_MAP.find(newToken->VALUE) != KEYWORD_MAP.end())
        newToken->TOKEN_TYPE = KEYWORD_MAP[newToken->VALUE];

    return newToken;
}

TOKEN *Lexer::tokenizeNUMBER()
{
    TOKEN *newToken = new TOKEN;
    newToken->position = cursor;
    string temporaryBuffer = "";
    bool decimal = false;
    while (isdigit(current) || current == '.')
    {
        if (current == '.')
            decimal = true;
        temporaryBuffer.push_back(current);
        advance();
    }

    // 9. -> float WE NEED TO HANDLE THIS

    newToken->TOKEN_TYPE = decimal ? TOKEN_FLOAT_DATA : TOKEN_INT_DATA;
    newToken->VALUE = temporaryBuffer;

    return newToken;
}

void Lexer::displayAllTokens()
{
    int counter = 0;
    for (TOKEN *CURRENT_TOKEN : TOKEN_LIST)
    {
        cout << ++counter << ") " << CURRENT_TOKEN->VALUE << " ";
        cout << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << endl;
    }
}

TOKEN *Lexer::tokenizeSPECIAL(TOKEN_SET NEW_TOKEN_TYPE)
{
    TOKEN *newToken = new TOKEN;
    newToken->position = cursor;
    newToken->TOKEN_TYPE = NEW_TOKEN_TYPE;
    newToken->VALUE = current;
    advance();
    return newToken;
}

LEXER_STATUS Lexer::throwLexerError()
{
    cout << FAIL << "\n[!] Lexer Error : Unidentified Character At Index " << cursor << " : " << current << endl
         << endl;
    return LEXER_FAIL;
}

LEXER_STATUS Lexer::throwStringParsingError()
{
    cout << FAIL << "[!] LEXER ERROR : CLOSING QUOTES NOT FOUND IN THE STRING PRESENT IN THE GIVEN COMMAND " << cursor << " : " << current << endl;
    return LEXER_FAIL;
}

LEXER_STATUS Lexer::tokenize()
{
    while (current)
    {
        skipWhiteSpaces();
        if (isalpha(current) || current == '_')
            TOKEN_LIST.push_back(tokenizeID());
        else if (isdigit(current))
            TOKEN_LIST.push_back(tokenizeNUMBER());
        else
        {
            switch (current)
            {
            case '(':
            {
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_LEFT_PAREN));
                break;
            }
            case ')':
            {
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PAREN));
                break;
            }
            case ',':
            {
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_COMMA));
                break;
            }
            case '.':
            {
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_DOT));
                break;
            }

                // replace all the advance functions with the seek function

            case '<':
            {
                if (seek(1) == '=') // <=
                {
                    advance();
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_LESS_THAN_EQUALS));
                }
                else
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_LESS_THAN));
                break;
            }
            case '>':
            {
                if (seek(1) == '=') // >=
                {
                    advance();
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_GREATER_THAN_EQUALS));
                }
                else
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_GREATER_THAN));
                break;
            }

            case '=':
            {
                if (seek(1) == '=')
                {
                    advance();
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                }
                else
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_EQUAL_TO));
                break;
            }
            case '"':
            {
                TOKEN_LIST.push_back(tokenizeSTRING());
                if (stringParsingError)
                    return throwStringParsingError();
                break;
            }
            case '[':
            {
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_LEFT_SQR_BRACKET));
                break;
            }
            case ']':
            {
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_RIGHT_SQR_BRACKET));
                break;
            }
            case '!':
            {
                if (seek(1) == '=')
                {
                    advance();
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_NOT_EQUALS));
                }
                else
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_NOT));
                break;
            }
            case ':':
            {
                if (seek(1) == ':')
                {
                    advance();
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_DOUBLE_COLON));
                }
                else
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_COLON));
                break;
            }
            case '|':
            {
                advance();
                if (current != '|')
                    return throwLexerError();
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_OR));
                break;
            }
            case '&':
            {
                advance();
                if (current != '&')
                    return throwLexerError();
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_AND));
                break;
            }
            case '-':
            {
                advance();
                if (current != '>')
                    return throwLexerError();
                TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_ARROW));
                break;
            }
            case '\0':
                break;
            default:
            {
                return throwLexerError();
            }
            }
        }
    }
    TOKEN *END_TOKEN = new TOKEN;
    END_TOKEN->TOKEN_TYPE = TOKEN_END_OF_INPUT;
    TOKEN_LIST.push_back(END_TOKEN);
    return LEXER_SUCCESS;
}

vector<TOKEN *> *Lexer::getTokenStream()
{
    displayAllTokens();
    return &TOKEN_LIST;
}



void Lexer::initialize(string InputBuffer)
{
    cursor = 0;
    length = InputBuffer.size();
    LocalInputBuffer = InputBuffer;
    current = LocalInputBuffer[cursor];
    TOKEN_LIST.clear();
    stringParsingError = false;
}

char Lexer::seek(int offset)
{
    if (cursor + offset >= length)
        return '\0';
    else
        return LocalInputBuffer[cursor + offset];
}