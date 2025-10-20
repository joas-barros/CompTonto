#include "symbol_table.h"

void SymbolTable::addSymbol(const string& name, int token, int line, int column) {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        it->second.locations.push_back({line, column});
    } else {
        Symbol sym;
        sym.name = name;
        sym.token = token;
        sym.locations.push_back({line, column});
        symbols[name] = sym;
    }
}

const Symbol* SymbolTable::getSymbol(const string& name) const {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return &it->second;
    } else {
        return nullptr; 
    }
}

int SymbolTable::countTokensOccurrences(int token) const {
    int count = 0;
    for (const auto& pair : symbols) {
        const Symbol& sym = pair.second;
        if (sym.token == token) {
            count += 1;
        }
    }
    return count;
}

const unordered_map<string, Symbol>& SymbolTable::getSymbols() const {
    return symbols;
}
