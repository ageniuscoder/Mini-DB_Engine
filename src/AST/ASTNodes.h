#ifndef ASTNODES_H
#define ASTNODES_H

#include <string>
#include <vector>
#include "AST.h"
using namespace std;

class CreateTable : public StatementNode{
    private:
    string m_tableName;
    vector<ColumnDefinition> m_columns;

    public:
    CreateTable(const string &tableName,const vector<ColumnDefinition> &columns){
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

class SelectTable : public StatementNode {
    private:
    string m_tableName;
    vector<string> m_columns;
    unique_ptr<ExpressionNode> m_whereClause;

    public:
    SelectTable(const string &tableName,const vector<string> &columns, unique_ptr<ExpressionNode> whereClause){
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

#endif