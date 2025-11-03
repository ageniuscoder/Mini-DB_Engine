#include "evaluationWrapper.h"

EvaluationWrapper::EvaluationWrapper()
{
    MAIN_LEXER = new Lexer();
    MAIN_PARSER = new Parser();
}

AST_NODE *EvaluationWrapper::handle(string InputBuffer)
{
    MAIN_LEXER->initialize(InputBuffer);
    LEXER_STATUS CURRENT_LEXER_STATUS = MAIN_LEXER->tokenize();

    if (CURRENT_LEXER_STATUS == LEXER_SUCCESS)
    {
        PARSER_STATUS CURRENT_PARSER_STATUS;
        MAIN_PARSER->initialize(MAIN_LEXER->getTokenStream(), InputBuffer);
        CURRENT_PARSER_STATUS = MAIN_PARSER->parse();
        if (MAIN_PARSER->error_level == 1) // HANDLING PARSER FAIL
            return nullptr;
        else
            return MAIN_PARSER->EVALUATED_NODE;
    }
    else // HANDLING LEXER FAIL
        return nullptr;

    return nullptr; // dummy nullptr return
}