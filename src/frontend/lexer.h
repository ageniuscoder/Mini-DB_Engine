#pragma once
#ifndef __LEXER_H
#define __LEXER_H
#include <bits/stdc++.h>
using namespace std;

typedef enum
{
    LEXER_FAIL,
    LEXER_SUCCESS,
} LEXER_STATUS;

typedef enum
{
    TOKEN_INSERT,
    TOKEN_INTO,
    TOKEN_VALUE,
    TOKEN_DELETE,
    TOKEN_FROM,
    TOKEN_SEARCH,
    TOKEN_IN,
    TOKEN_CREATE,
    TOKEN_NEW,
    TOKEN_DATABASE,
    TOKEN_TABLE,
    TOKEN_USE,
    TOKEN_UPDATE,
    TOKEN_WHERE,
    TOKEN_WITH,
    TOKEN_EXIT,

    TOKEN_STRING, // this would be for the name of ad and stuff
    TOKEN_INTEGER,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_QUOTES,
    TOKEN_COMMA,
    TOKEN_EQUALS,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
} TOKEN_SET;

struct TOKEN
{
    TOKEN_SET TOKEN_TYPE;
    string VALUE;
};

string tokenTypeToString(TOKEN_SET REQUIRED_TOKEN)
{
    switch (REQUIRED_TOKEN)
    {
    case TOKEN_INSERT:
        return "TOKEN_INSERT";
    case TOKEN_INTO:
        return "TOKEN_INTO";
    case TOKEN_VALUE:
        return "TOKEN_VALUE";
    case TOKEN_DELETE:
        return "TOKEN_DELETE";
    case TOKEN_FROM:
        return "TOKEN_FROM";
    case TOKEN_SEARCH:
        return "TOKEN_SEARCH";
    case TOKEN_IN:
        return "TOKEN_IN";
    case TOKEN_CREATE:
        return "TOKEN_CREATE";
    case TOKEN_NEW:
        return "TOKEN_NEW";
    case TOKEN_DATABASE:
        return "TOKEN_DATABASE";
    case TOKEN_TABLE:
        return "TOKEN_TABLE";
    case TOKEN_USE:
        return "TOKEN_USE";
    case TOKEN_UPDATE:
        return "TOKEN_UPDATE";
    case TOKEN_WHERE:
        return "TOKEN_WHERE";
    case TOKEN_WITH:
        return "TOKEN_WITH";
    case TOKEN_EXIT:
        return "    TOKEN_EXIT";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_INTEGER:
        return "TOKEN_INTEGER";
    case TOKEN_LEFT_PAREN:
        return "TOKEN_LEFT_PAREN";
    case TOKEN_RIGHT_PAREN:
        return "TOKEN_RIGHT_PAREN";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_QUOTES:
        return "TOKEN_QUOTES";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_LESS_THAN:
        return "TOKEN_LESS_THAN";
    case TOKEN_GREATER_THAN:
        return "TOKEN_GREATER_THAN";
    default:
        return "Unindentified Token!";
    }
    return "[!] ERROR :  UNIDENTIFIED TOKEN : " + REQUIRED_TOKEN;
};

unordered_map<string, TOKEN_SET> KEYWORD_MAP = {
    // for lowercase
    {"insert", TOKEN_INSERT},
    {"into", TOKEN_INTO},
    {"value", TOKEN_VALUE},
    {"delete", TOKEN_DELETE},
    {"from", TOKEN_FROM},
    {"search", TOKEN_SEARCH},
    {"in", TOKEN_IN},
    {"create", TOKEN_CREATE},
    {"new", TOKEN_NEW},
    {"database", TOKEN_DATABASE},
    {"table", TOKEN_TABLE},
    {"use", TOKEN_USE},
    {"update", TOKEN_UPDATE},
    {"where", TOKEN_WHERE},
    {"with", TOKEN_WITH},
    {"exit", TOKEN_EXIT},

    // for uppercase
    {"INSERT", TOKEN_INSERT},
    {"INTO", TOKEN_INTO},
    {"VALUE", TOKEN_VALUE},
    {"DELETE", TOKEN_DELETE},
    {"FROM", TOKEN_FROM},
    {"SEARCH", TOKEN_SEARCH},
    {"IN", TOKEN_IN},
    {"CREATE", TOKEN_CREATE},
    {"NEW", TOKEN_NEW},
    {"DATABASE", TOKEN_DATABASE},
    {"TABLE", TOKEN_TABLE},
    {"USE", TOKEN_USE},
    {"UPDATE", TOKEN_UPDATE},
    {"WHERE", TOKEN_WHERE},
    {"WITH", TOKEN_WITH},
    {"EXIT", TOKEN_EXIT},
};

class Lexer
{
private:
    int cursor;
    int length;
    char current;
    string localInputBuffer;
    vector<TOKEN *> TOKEN_LIST;

    char advance();
    void skipWhiteSpaces();
    TOKEN *tokenizeAlpha();
    TOKEN *tokenizeInteger();
    TOKEN *tokenizeSpecial(TOKEN_SET NEW_TOKEN_TYPE);

public:
    Lexer(); // this is main lexer constructor
    void intialize(string inputBuffer);
    void displayAllTokens();
    LEXER_STATUS tokenize();
    LEXER_STATUS throwLexerError();
};

#endif
