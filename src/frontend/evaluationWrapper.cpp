#include <bits/stdc++.h>
#include "evaluationWrapper.h"
using namespace std;

EvaluationWrapper::EvaluationWrapper()
{
    MAIN_LEXER = new Lexer();
    commandCount = 0;
}

void EvaluationWrapper::handle(string inputBuffer)
{
    // record the time of the command here using a cmmand function
    // track histry of commands
    // maybe use a boolean switch
    MAIN_LEXER->intialize(inputBuffer);
    LEXER_STATUS CURRENT_STATUS = MAIN_LEXER->tokenize();
    commandCount++;
    // add colors to the prompt
    if (CURRENT_STATUS == LEXER_SUCCESS)
    {
        cout << "$ Command ID -> " << commandCount << " : " << "executed in 0.2ms \n\n";
    }
    else if (CURRENT_STATUS == LEXER_FAIL)
    {
        cout << "$ Command ID -> " << commandCount << " : " << "failed in 0.2ms\n\n";
    }
}