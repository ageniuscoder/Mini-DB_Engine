#include <bits/stdc++.h>
#include "evaluationWrapper.h"
using namespace std;

#define FAIL "\e[0;31m"
#define SUCCESS "\e[0;32m"
#define DEFAULT "\e[0;37m"

EvaluationWrapper::EvaluationWrapper()
{
    MAIN_LEXER = new Lexer();
    commandCount = 0;
}

void EvaluationWrapper::handle(string inputBuffer)
{
    auto startTimer = chrono::high_resolution_clock::now();
    MAIN_LEXER->intialize(inputBuffer);
    LEXER_STATUS CURRENT_STATUS = MAIN_LEXER->tokenize();
    auto endTimer = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTimer - startTimer).count();
    commandCount++;
    if (CURRENT_STATUS == LEXER_SUCCESS)
    {
        cout << SUCCESS << "$ Command ID -> " << commandCount << " : " << "executed in " << duration << "ms" << " \n\n"
             << DEFAULT;
    }
    else if (CURRENT_STATUS == LEXER_FAIL)
    {
        cout << FAIL << "$ Command ID -> " << commandCount << " : " << "failed in " << duration << "ms" << " \n\n"
             << DEFAULT;
    }
}