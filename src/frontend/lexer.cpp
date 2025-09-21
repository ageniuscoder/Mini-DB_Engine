#include <bits/stdc++.h>
#include "lexer.h"
using namespace std;

#define FAIL "\e[0;31m"
#define DEFAULT "\e[0;37m"

Lexer::Lexer() {}

void Lexer::skipWhiteSpaces() // deal with end whitespaces in the cmmand
{
    while (current == ' ' && current != '\0')
    {
        advance();
    }
}

LEXER_STATUS Lexer::tokenize()
{
    while (current)
    {
        skipWhiteSpaces();
        if (isalpha(current))
        {
            TOKEN_LIST.push_back(tokenizeID());
        }
        else if (isdigit(current))
        {
            TOKEN_LIST.push_back(tokenizeInteger());
        }
        else
        {
            switch (current)
            {
            case '(':
            {
                TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_LEFT_PAREN));
                break;
            }
            case ')':
            {
                TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_RIGHT_PAREN));
                break;
            }
            case ',':
            {
                TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_COMMA));
                break;
            }
            case '=':
            {
                advance();
                if (current != '=')
                {
                    return throwLexerError();
                }
                TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_EQUALS));
                break;
            }
            case '<':
            {
                TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_LESS_THAN));
                break;
            }
            case '>':
            {
                TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_GREATER_THAN));
                break;
            }
            case '"':
            {
                TOKEN_LIST.push_back(tokenizeString());
                if (stringParsingError)
                {
                    return throwStringParsingError();
                }
                break;
            }
            case '\0':
            {
                break;
            }
            default:
            {
                return throwLexerError();
            }
            }
        }
    }
    displayAllTokens();
    TOKEN *END_TOKEN = new TOKEN;
    END_TOKEN->TOKEN_TYPE = TOKEN_END_OF_INPUT;
    TOKEN_LIST.push_back(END_TOKEN);
    return LEXER_SUCCESS;
}

void Lexer::intialize(string inputBuffer)
{
    cursor = 0;
    length = inputBuffer.length();
    localInputBuffer = inputBuffer;
    current = localInputBuffer[cursor];
    TOKEN_LIST.clear();
    stringParsingError = false;
}

char Lexer::advance()
{
    if (cursor == length - 1) // this means we are at the end of the input buffer
    {
        current = '\0';
    }
    else
    {
        current = localInputBuffer[++cursor];
    }
    return current;
}

vector<TOKEN *> *Lexer::getTokenStream()
{
    return &TOKEN_LIST;
}

TOKEN *Lexer::tokenizeString()
{
    advance(); // advancing the opening quotes
    TOKEN *newToken = new TOKEN;
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
    newToken->TOKEN_TYPE = TOKEN_STRING;
    newToken->VALUE = temporaryBuffer;
    return newToken;
}

TOKEN *Lexer::tokenizeID()
{
    TOKEN *newToken = new TOKEN;
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
    {
        newToken->TOKEN_TYPE = KEYWORD_MAP[newToken->VALUE];
    }
    return newToken;
}

TOKEN *Lexer::tokenizeInteger()
{
    TOKEN *newToken = new TOKEN;
    string temporaryBuffer = "";

    while (isdigit(current) || current == '_')
    {
        temporaryBuffer.push_back(current);
        advance();
    }
    newToken->TOKEN_TYPE = TOKEN_INTEGER;
    newToken->VALUE = temporaryBuffer;

    if (KEYWORD_MAP.find(newToken->VALUE) != KEYWORD_MAP.end())
    {
        newToken->TOKEN_TYPE = KEYWORD_MAP[newToken->VALUE];
    }

    return newToken;
}

TOKEN *Lexer::tokenizeSpecial(TOKEN_SET NEW_TOKEN_TYPE)
{
    TOKEN *newToken = new TOKEN;
    newToken->TOKEN_TYPE = NEW_TOKEN_TYPE;
    if (newToken->TOKEN_TYPE == TOKEN_EQUALS)
    {
        newToken->VALUE = "==";
    }
    else
    {
        newToken->VALUE = current;
    }
    advance();
    return newToken;
}

void Lexer::displayAllTokens()
{
    int counter = 0;
    for (TOKEN *CURRENT_TOKEN : TOKEN_LIST)
    {
        cout << ++counter << ")" << CURRENT_TOKEN->VALUE << " " << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << endl;
    }
}

LEXER_STATUS Lexer::throwLexerError()
{
    cout << FAIL << "[!] LEXER ERROR : UNIDENTIFIED CHARACTER  AT INDEX: " << cursor << " : " << current << DEFAULT << endl;
    return LEXER_FAIL;
}

LEXER_STATUS Lexer::throwStringParsingError()
{
    cout << FAIL << "[!] LEXER ERROR : CLOSING QUOTES NOT FOUND IN THE STRING PRESENT IN GIVEN COMMAND :" << cursor << "  " << current << DEFAULT << endl;
    return LEXER_FAIL;
}