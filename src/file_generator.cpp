#include "file_generator.h"

string getTokenName(int token) {
    switch (token) {
        case PALAVRA_RESERVADA: return "PALAVRA_RESERVADA";
        case DADO_NATIVO:       return "DADO_NATIVO";
        case META_ATRIBUTO:     return "META_ATRIBUTO";
        case ESTERIOTIPO_CLASSE: return "ESTERIOTIPO_CLASSE";
        case ESTERIOTIPO_RELACAO: return "ESTERIOTIPO_RELACAO";
        case NOVO_TIPO:         return "NOVO_TIPO";
        case NOME_DE_CLASSE:    return "NOME_DE_CLASSE";
        case NOME_DE_RELACAO:   return "NOME_DE_RELACAO";
        case INSTANCIA:         return "INSTANCIA";
        case NUMERO:            return "NUMERO";
        case IMPORT:           return "IMPORT";
        case PACKAGE:          return "PACKAGE";
        case SPECIFICS:         return "SPECIFICS";
        case SPECIALIZES:      return "SPECIALIZES";
        case WHERE:            return "WHERE";
        case ENUM:             return "ENUM";
        case RELATION:         return "RELATION";
        case DATATYPE:         return "DATATYPE";
        case GENSET :          return "GENSET";
        case DISJOINT:         return "DISJOINT";
        case COMPLETE:         return "COMPLETE";
        case GENERAL:          return "GENERAL";
        case RELATION_KEYWORD: return "RELATION_KEYWORD";
        case LEFT_CURLY_BRACKETS: return "LEFT_CURLY_BRACKETS";
        case RIGHT_CURLY_BRACKETS: return "RIGHT_CURLY_BRACKETS";
        case LEFT_PARENTHESIS: return "LEFT_PARENTHESIS";
        case RIGHT_PARENTHESIS: return "RIGHT_PARENTHESIS";
        case LEFT_SQUARE_BRACKETS: return "LEFT_SQUARE_BRACKETS";
        case RIGHT_SQUARE_BRACKETS: return "RIGHT_SQUARE_BRACKETS";
        case COMMA: return "COMMA";
        case AT: return "AT";
        case COLON: return "COLON";
        case DOUBLEDOT: return "DOUBLEDOT";
        case ASTERISK: return "ASTERISK";
        case NAO_IDENTIFICADO:  return "NAO_IDENTIFICADO";
        default:                return "UNKNOWN (" + to_string(token) + ")";
    }
}

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

        const string tokenStr = getTokenName(sym.token);


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
    fout << "Contagem da quantidade de ocorrências únicas de cada token:" << endl;
    fout << "==========================" << endl;

    fout << "Total de palavras reservadas: " << symbolTable.countTokensOccurrences(PALAVRA_RESERVADA) << endl;
    fout << "Total de classes: " << symbolTable.countTokensOccurrences(NOME_DE_CLASSE) << endl;
    fout << "Total de esteriotipos de classes: " << symbolTable.countTokensOccurrences(ESTERIOTIPO_CLASSE) << endl;
    fout << "Total de relações: " << symbolTable.countTokensOccurrences(NOME_DE_RELACAO) << endl;
    fout << "Total de esteriotipos de relações: " << symbolTable.countTokensOccurrences(ESTERIOTIPO_RELACAO) << endl;
    fout << "Total de instâncias: " << symbolTable.countTokensOccurrences(INSTANCIA) << endl;
    fout << "Total de meta atributos: " << symbolTable.countTokensOccurrences(META_ATRIBUTO) << endl;

    fout << "==========================" << endl;

    fout.close();

    cout << YELLOW_TEXT << "Relatório da tabela de símbolos gerado em output/symbol_report.txt" << RESET_COLOR << endl;
}

void FileGenerator::generateErrorReport(const string& filename) {
    int numErrors = symbolTable.countTokensOccurrences(NAO_IDENTIFICADO);

    fout.open(filename);
    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo error_report.txt para escrita." << RESET_COLOR << endl;
        return;
    }

    if (numErrors == 0) {
        fout << "Nenhum erro léxico encontrado! :)" << endl;
        fout.close();

        cout << GREEN_TEXT << "Nenhum erro léxico encontrado e reportado em output/error_report.txt" << RESET_COLOR << endl;
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
