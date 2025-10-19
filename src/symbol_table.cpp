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

void SymbolTable::exportJson() const {
    ofstream fout;

    fout.open("output/symbol_table.json");
    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo symbol_table.json para escrita." << RESET_COLOR << endl;
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
        
        fout << "      \"locations\": [";
        for (size_t i = 0; i < sym.locations.size(); ++i) {
            if (i > 0) fout << ", ";
            fout << "{\"line\": " << sym.locations[i].line << ", \"col\": " << sym.locations[i].column << "}";
        }
        fout << "]\n";
        
        fout << "    }";
    }
    fout << "\n  ]\n";
    fout << "}\n";
    fout.close();

    cout << YELLOW_TEXT << "Tabela de símbolos exportada para output/symbol_table.json" << RESET_COLOR << endl;
}

int SymbolTable::countTokensOccurrences(int token) const {
    int count = 0;
    for (const auto& pair : symbols) {
        const Symbol& sym = pair.second;
        if (sym.token == token) {
            count += sym.locations.size();
        }
    }
    return count;
}

void SymbolTable::generateReport() const {
    ofstream fout;

    fout.open("output/symbol_report.txt");
    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo symbol_report.txt para escrita." << RESET_COLOR << endl;
        return;
    }

    fout << "RELATÓRIO DE ANÁLISE LÉXICA" << endl;
    fout << "==========================" << endl;

    fout << "Total de palavras reservadas: " << countTokensOccurrences(PALAVRA_RESERVADA) << endl;
    fout << "Total de classes: " << countTokensOccurrences(NOME_DE_CLASSE) << endl;
    fout << "Total de relações: " << countTokensOccurrences(NOME_DE_RELACAO) << endl;
    fout << "Total de instâncias: " << countTokensOccurrences(INSTANCIA) << endl;
    fout << "Total de meta atributos: " << countTokensOccurrences(META_ATRIBUTO) << endl;

    fout << "==========================" << endl;

    fout.close();

    cout << YELLOW_TEXT << "Relatório da tabela de símbolos gerado em output/symbol_report.txt" << RESET_COLOR << endl;
}