#pragma once
#ifndef __EVALUATIONWRAPPER_H
#define __EVALUATIONWRAPPER_H
#include "lexer.cpp"

class EvaluationWrapper
{
    private:
    Lexer* MAIN_LEXER;
    int commandCount;

    public:
    EvaluationWrapper(); // constructor
    void handle(string inputBuffer);
};

#endif