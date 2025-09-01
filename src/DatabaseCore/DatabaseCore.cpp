#include "DatabaseCore.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../Utils/Exceptions.h"
#include "../AST/ASTNodes.h"
using namespace std;

namespace fs=filesystem;

string DatabaseCore::getFilePath(const string& tableName) {
    return "data/tables/" + tableName + ".txt";
}

void DatabaseCore::createTable(const string &tableName, const vector<ColumnDefinition> &columns){
    string filePath=getFilePath(tableName);

    if(fs::exists(filePath)){
        throw CustomException("Table '" + tableName + "' already exists.");
    }

    ofstream outFile(filePath);

    if(outFile.is_open()){
        throw CustomException("Failed to create Table '" + tableName +"' .");
    }

    // Write the column headers.
    for (size_t i = 0; i < columns.size(); ++i) {
        outFile << columns[i].name;
        if (i < columns.size() - 1) {
            outFile << ",";
        }
    }
    outFile << "\n";
    outFile.close();
}