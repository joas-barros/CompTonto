#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include "symbol_table.h"
#include "synthesis_table.h"
#include "colors.h"
#include "parser.hpp"

using namespace std;

string getTokenName(int token);
class FileGenerator{
private:
    const SymbolTable& symbolTable;
    const SynthesisTable& synthesisTable;
    ofstream fout;
public:
    FileGenerator(const SymbolTable& symTable, const SynthesisTable& synthTable);
    ~FileGenerator();
    void generateSymbolTableJson(const string& filename);
    void generateSymbolReport(const string& filename);
    void generateErrorReport(const string& filename);
    void generateSynthesisTableJson(const string& filename);
    void generateSynthesisReport(const string& filename);
};

#endif // FILE_GENERATOR_H