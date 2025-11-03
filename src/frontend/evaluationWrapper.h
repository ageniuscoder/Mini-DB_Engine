#ifndef __EVALUATION_WRAPPER_H
#define __EVALUATION_WRAPPER_H

#include "parser.cpp"

class EvaluationWrapper
{
private:
    Lexer *MAIN_LEXER;
    Parser *MAIN_PARSER;

public:
EvaluationWrapper();
AST_NODE *handle(string InputBuffer);
};

#endif
