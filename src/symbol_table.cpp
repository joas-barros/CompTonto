#include "symbol_table.h"

void SymbolTable::addSymbol(const string& name, int token, int line) {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        it->second.lines.push_back(line);
    } else {
        Symbol sym;
        sym.name = name;
        sym.token = token;
        sym.lines.push_back(line);
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