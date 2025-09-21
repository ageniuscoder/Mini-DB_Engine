#pragma once
#ifndef __EVALUATIONWRAPPER_H
#define __EVALUATIONWRAPPER_H
#include "lexer.cpp"
#include "parser.cpp"

class EvaluationWrapper
{
    private:
    Lexer* MAIN_LEXER;
    Parser* MAIN_PARSER;
    int commandCount;

    public:
    EvaluationWrapper(); // constructor
    void handle(string inputBuffer);
};

#endif