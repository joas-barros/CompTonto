#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include "symbol_table.h"
#include "colors.h"
#include "parser.hpp"

using namespace std;

string getTokenName(int token);
class FileGenerator{
private:
    const SymbolTable& symbolTable;
    ofstream fout;
public:
    FileGenerator(const SymbolTable& symTable);
    ~FileGenerator();
    void generateSymbolTableJson(const string& filename);
    void generateSymbolReport(const string& filename);
    void generateErrorReport(const string& filename);
};

#endif // FILE_GENERATOR_H