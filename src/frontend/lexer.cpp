#include <bits/stdc++.h>
#include "lexer.h"
using namespace std;

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
            TOKEN_LIST.push_back(tokenizeAlpha());
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
            default:
            {
                return throwLexerError();
            }
            }
        }
    }
    displayAllTokens();
    return LEXER_SUCCESS;
}

void Lexer::intialize(string inputBuffer)
{
    cursor = 0;
    length = inputBuffer.length();
    localInputBuffer = inputBuffer;
    current = localInputBuffer[cursor];
    TOKEN_LIST.clear();
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

TOKEN *Lexer::tokenizeAlpha()
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

    newToken->TOKEN_TYPE = TOKEN_STRING;
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
    newToken->VALUE = current;
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
    cout << "[!] LEXER ERROR : UNIDENTIFIED CHARACTER  AT INDEX: " << cursor << " : " << current << endl;
    return LEXER_FAIL;
}