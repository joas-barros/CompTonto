#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
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
    void addSymbol(const string& name, Token type);
    Token getType(const string& name) const;

private:
    unordered_map<string, Symbol> symbols;
};

#endif // SYMBOL_TABLE_H