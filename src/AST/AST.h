#ifndef AST_H
#define AST_H

#include "../Utils/Types.h"
#include <memory>
#include <string>

//base class for all sql statement nodes in AST
class StatementNode{
    public:
    virtual ~StatementNode() = default;
};


//base class for all expression nodes(ex:- for WHERE clause)
class ExpressionNode{
    public:
    virtual ~ExpressionNode() = default;
};

#endif