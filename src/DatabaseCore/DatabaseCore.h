#ifndef DATABASECORE_H
#define DATABASECORE_H

#include <string>
#include <vector>
#include <memory>
#include "../AST/AST.h"
#include "../Utils/Types.h"
using namespace std;

class DatabaseCore {
    private:

    //for building the full file path.
    string getFilePath (const string &tableName);

    //for checking if a record matches a WHERE clause.
    bool evaluateWhereClause(const vector<string> &record,const unique_ptr<ExpressionNode> &WhereClause);

    public:
    //CRUD OPERATIONS
    void createTable(const string &tableName, const vector<ColumnDefinition> &columns);

    void insertRecord(const string &tableName, const vector<string> &values);

    void selectRecord(const string &tableName, const vector<string> &columns,const unique_ptr<ExpressionNode> &whereClause);

    void deleteRecord(const string &tableName, const unique_ptr<ExpressionNode> &whereClause);

};

#endif