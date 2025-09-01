#ifndef ASTNODES_H
#define ASTNODES_H

#include <string>
#include <vector>
#include "AST.h"
using namespace std;

// Concrete node for a CREATE TABLE statement.
class CreateTableNode : public StatementNode{
    private:
    string m_tableName;
    vector<ColumnDefinition> m_columns;

    public:
    CreateTableNode(const string &tableName,const vector<ColumnDefinition> &columns){
        m_tableName=tableName;
        m_columns=columns;
    }

    const string &getTableName(){
        return m_tableName;
    }

    const vector<ColumnDefinition> &getColumns(){
        return m_columns;
    }

};

// Concrete node for a  SELECT statement.
class SelectNode : public StatementNode {
    private:
    string m_tableName;
    vector<string> m_columns;
    unique_ptr<ExpressionNode> m_whereClause;

    public:
    SelectNode(const string &tableName,const vector<string> &columns, unique_ptr<ExpressionNode> whereClause){
        m_tableName=tableName;
        m_columns=columns;
        m_whereClause=move(whereClause);
    }

    const string &getTableName(){
        return m_tableName;
    }

    const vector<string> &getColumns(){
        return m_columns;
    }

    const unique_ptr<ExpressionNode> &getWhereClause(){
        return m_whereClause;
    }
};

// Concrete node for a INSERT statement.
class InsertNode : public StatementNode{
    private:
    string m_tableName;
    vector<string> m_values;

    public:
    InsertNode(const string &tableName, const vector<string> &values){
        m_tableName=tableName;
        m_values=values;
    }

    const string &getTableName(){
        return m_tableName;
    }

    const vector<string> &getValues(){
        return m_values;
    }
};


// Concrete node for a DELETE statement.
class DeleteNode : public StatementNode {
    private:
    string m_tableName;
    unique_ptr<ExpressionNode> m_whereClause;

    public:
    DeleteNode(const string &tableName, unique_ptr<ExpressionNode> whereClause){
        m_tableName=tableName;
        m_whereClause=move(whereClause);
    }

    const string &getTableName(){
        return m_tableName;
    }

    const unique_ptr<ExpressionNode> &getWhereClause(){
        return m_whereClause;
    }
};


// Concrete node for a comparison expression (part of Where Clause).
class ComparisionNode : public ExpressionNode {
    private:
    string m_column;
    string m_op;
    string m_value;
    TokenType m_valueType;

    public:
    ComparisionNode(const string &column,const string &op, const string &value,TokenType valueType){
        m_column=column;
        m_op=op;
        m_value=value;
        m_valueType=valueType;
    }

    const string &getColumnName(){
        return m_column;
    }

    const string &getOp(){
        return m_op;
    }

    const string &getValue(){
        return m_value;
    }

    TokenType getValueType(){
        return m_valueType;
    }
};

#endif