#include "file_generator.h"

FileGenerator::FileGenerator(const SymbolTable& symTable) : symbolTable(symTable) {}

FileGenerator::~FileGenerator() {}

void FileGenerator::generateSymbolTableJson(const string& filename) {
    fout.open(filename);

    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo symbol_table.json para escrita." << RESET_COLOR << endl;
        return;
    }

    fout << "{\n";
    fout << "  \"symbols\": [\n";
    bool first = true;
    for (const auto& pair : symbolTable.getSymbols()) {
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

void FileGenerator::generateSymbolReport(const string& filename) {
    fout.open(filename);
    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo symbol_report.txt para escrita." << RESET_COLOR << endl;
        return;
    }

    fout << "RELATÓRIO DE ANÁLISE LÉXICA" << endl;
    fout << "==========================" << endl;

    fout << "Total de palavras reservadas: " << symbolTable.countTokensOccurrences(PALAVRA_RESERVADA) << endl;
    fout << "Total de classes: " << symbolTable.countTokensOccurrences(NOME_DE_CLASSE) << endl;
    fout << "Total de relações: " << symbolTable.countTokensOccurrences(NOME_DE_RELACAO) << endl;
    fout << "Total de instâncias: " << symbolTable.countTokensOccurrences(INSTANCIA) << endl;
    fout << "Total de meta atributos: " << symbolTable.countTokensOccurrences(META_ATRIBUTO) << endl;

    fout << "==========================" << endl;

    fout.close();

    cout << YELLOW_TEXT << "Relatório da tabela de símbolos gerado em output/symbol_report.txt" << RESET_COLOR << endl;
}

void FileGenerator::generateErrorReport(const string& filename) {
    int numErrors = symbolTable.countTokensOccurrences(NAO_IDENTIFICADO);

    if (numErrors == 0) {
        return; 
    }

    fout.open(filename);
    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo error_report.txt para escrita." << RESET_COLOR << endl;
        return;
    }

    fout << "RELATÓRIO DE ERROS LÉXICOS" << endl;
    fout << "==========================" << endl;

    for (const auto& pair : symbolTable.getSymbols()) {
        const Symbol& sym = pair.second;
        if (sym.token == NAO_IDENTIFICADO) {
            for (const auto& loc : sym.locations) {
                fout << "Erro Léxico: Símbolo não identificado '" << sym.name << "' na linha " << loc.line << ", coluna " << loc.column << endl;
            }
        }
    }

    fout << "==========================" << endl;
    fout.close();

    cout << RED_TEXT << numErrors << " erros léxicos encontrados e reportados em output/error_report.txt" << RESET_COLOR << endl;
}
