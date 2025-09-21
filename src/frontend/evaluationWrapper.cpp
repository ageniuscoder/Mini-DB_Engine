#include <bits/stdc++.h>
#include "evaluationWrapper.h"
using namespace std;

#define FAIL "\e[0;31m"
#define SUCCESS "\e[0;32m"
#define DEFAULT "\e[0;37m"

EvaluationWrapper::EvaluationWrapper()
{
    MAIN_LEXER = new Lexer();
    MAIN_PARSER = new Parser();
    commandCount = 0;
}

void EvaluationWrapper::handle(string inputBuffer)
{
    auto startTimer = chrono::high_resolution_clock::now();

    // using the lexer to tokenize the input buffer
    MAIN_LEXER->intialize(inputBuffer);
    LEXER_STATUS CURRENT_LEXER_STATUS = MAIN_LEXER->tokenize();

    // using the parser to parse the token stream
    PARSER_STATUS CURRENT_PARSER_STATUS;
    if (CURRENT_LEXER_STATUS == LEXER_SUCCESS)
    {
        MAIN_PARSER->intialize(MAIN_LEXER->getTokenStream());
        CURRENT_PARSER_STATUS = MAIN_PARSER->parse();
    }

    auto endTimer = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTimer - startTimer).count();

    commandCount++;

    if (CURRENT_LEXER_STATUS == LEXER_FAIL || CURRENT_PARSER_STATUS == PARSER_FAIL)
    {
        cout << FAIL << "$ Command ID -> " << commandCount << " : " << "failed in " << duration << "ms" << " \n\n"
             << DEFAULT;
    }
    else
    {
        cout << SUCCESS << "$ Command ID -> " << commandCount << " : " << "executed in " << duration << "ms" << " \n\n"
             << DEFAULT;
    }
}