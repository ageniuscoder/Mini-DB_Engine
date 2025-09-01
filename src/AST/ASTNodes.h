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

};

#endif