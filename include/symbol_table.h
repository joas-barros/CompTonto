#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "tokens.h"
#include "colors.h"

using namespace std;

// Estrutura para armazenar a localização de um token
struct Location {
    int line;
    int column;
};

// Estruturas de dados para a tabela de símbolos
struct Symbol {
    string name;
    int token;
    vector<Location> locations;
};


class SymbolTable {
public:
    void addSymbol(const string& name, int token, int line, int column);
    const Symbol* getSymbol(const string& name) const;
    void exportJson() const;
    void generateReport() const;
    int countTokensOccurrences(int token) const;
private:
    unordered_map<string, Symbol> symbols;
};

#endif // SYMBOL_TABLE_H