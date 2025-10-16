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

void SymbolTable::exportJson() const {
    ofstream fout;

    fout.open("output/symbol_table.json");
    if (!fout.is_open()) {
        cerr << "Erro ao abrir o arquivo symbol_table.json para escrita." << endl;
        return;
    }

    fout << "{\n";
    fout << "  \"symbols\": [\n";
    bool first = true;
    for (const auto& pair : symbols) {
        if (!first) {
            fout << ",\n";
        }
        first = false;
        const Symbol& sym = pair.second;

        const string tokenStr = tokenToString(static_cast<Token>(sym.token));

        fout << "    {\n";
        fout << "      \"name\": \"" << sym.name << "\",\n";
        fout << "      \"token\": \"" << tokenStr << "\",\n";
        fout << "      \"lines\": [";
        for (size_t i = 0; i < sym.lines.size(); ++i) {
            if (i > 0) fout << ", ";
            fout << sym.lines[i];
        }
        fout << "]\n";
        fout << "    }";
    }
    fout << "\n  ]\n";
    fout << "}\n";
    fout.close();

    cout << "Tabela de símbolos exportada para output/symbol_table.json" << endl;
}