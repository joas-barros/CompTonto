#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "tokens.h"

using namespace std;
    

// Estruturas de dados para a tabela de símbolos
struct Symbol {
    string name;
    int token;
    vector<int> lines;
};

class SymbolTable {
public:
    void addSymbol(const string& name, int token, int line);
    const Symbol* getSymbol(const string& name) const;
    void exportJson() const;

private:
    unordered_map<string, Symbol> symbols;
};

#endif // SYMBOL_TABLE_H