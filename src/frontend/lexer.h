#ifndef __LEXER_H
#define __LEXER_H

#include <bits/stdc++.h>
using namespace std;

#define FAIL "\e[0;31m"
#define SUCCESS "\e[0;32m"
#define DEFAULT "\e[0;37m"
#define BLUE "\e[1;34m"
#define YELLOW "\e[0;33m"
#define DB_PROMPT "Dharana $ "

typedef enum
{
    LEXER_SUCCESS,
    LEXER_FAIL
} LEXER_STATUS;

typedef enum
{
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_BOOL,
    TOKEN_DATE,
    TOKEN_TIME,
    TOKEN_NEW,
    TOKEN_DOUBLE_COLON,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_ADD,
    TOKEN_LEFT_SQR_BRACKET,
    TOKEN_RIGHT_SQR_BRACKET,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_STRING_DATA,
    TOKEN_INT_DATA,
    TOKEN_FLOAT_DATA,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_CREATE,
    TOKEN_USE,
    TOKEN_PRINT,
    TOKEN_REMOVE,
    TOKEN_SERVER,
    TOKEN_SERVER_CONNECT,
    TOKEN_SERVER_CREATE,
    TOKEN_UPDATE,
    TOKEN_NOT,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_ARROW,
    TOKEN_EQUAL_TO,
    TOKEN_EQUALS,
    TOKEN_NOT_EQUALS,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_LESS_THAN_EQUALS,
    TOKEN_GREATER_THAN_EQUALS,
    TOKEN_ID,
    TOKEN_EXIT,
    TOKEN_END_OF_INPUT,
    TOKEN_COLON,
    TOKEN_PRIMARY,
    TOKEN_EXPORT
} TOKEN_SET;

struct TOKEN
{
    TOKEN_SET TOKEN_TYPE;
    string VALUE;
    int position;
};

extern unordered_map<string, TOKEN_SET> KEYWORD_MAP;

extern string tokenTypeToString(TOKEN_SET REQUIRED_TOKEN);
class Lexer
{
private:
    int cursor, length;
    char current;
    string LocalInputBuffer;
    vector<TOKEN *> TOKEN_LIST;
    bool stringParsingError;

    char advance();
    void skipWhiteSpaces();
    TOKEN *tokenizeSTRING();
    TOKEN *tokenizeID();
    TOKEN *tokenizeNUMBER();
    TOKEN *tokenizeSPECIAL(TOKEN_SET);
    LEXER_STATUS throwLexerError();
    LEXER_STATUS throwStringParsingError();
    void displayAllTokens();

public:
    Lexer();
    void initialize(string InputBuffer);
    LEXER_STATUS tokenize();
    vector<TOKEN *> *getTokenStream();
    char seek(int offset);
};

#endif
