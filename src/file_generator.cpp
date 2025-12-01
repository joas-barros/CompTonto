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
        case OVERLAPPING:      return "OVERLAPPING";
        case INCOMPLETE:       return "INCOMPLETE";
        case GENERAL:          return "GENERAL";
        case LEFT_AGGREGATION_ARROW: return "LEFT_AGGREGATION_ARROW";
        case RIGHT_AGGREGATION_ARROW: return "RIGHT_AGGREGATION_ARROW";
        case LEFT_COMPOSITION_ARROW: return "LEFT_COMPOSITION_ARROW";
        case RIGHT_COMPOSITION_ARROW: return "RIGHT_COMPOSITION_ARROW";
        case GENERAL_RELATION_ARROW: return "GENERAL_RELATION_ARROW";
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

FileGenerator::FileGenerator(const SymbolTable& symTable, const SynthesisTable& synthTable) : symbolTable(symTable), synthesisTable(synthTable) {}

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

#include "file_generator.h"

// Implementação do método de exportação da Tabela de Síntese
void FileGenerator::generateSynthesisTableJson(const string& filename) {
    fout.open(filename);

    if (!fout.is_open()) {
        cerr << RED_TEXT << "Erro ao abrir o arquivo " << filename << " para escrita." << RESET_COLOR << endl;
        return;
    }

    fout << "{\n";
    
    // --- 1. PACOTES ---
    fout << "  \"packages\": [\n";
    
    const auto& packages = synthesisTable.getPackages();
    bool firstPkg = true;

    for (const auto& pkg : packages) {
        if (!firstPkg) fout << ",\n";
        firstPkg = false;

        fout << "    {\n";
        fout << "      \"name\": \"" << pkg.name << "\",\n";
        
        // --- 1.1 CLASSES DO PACOTE ---
        fout << "      \"classes\": [\n";
        bool firstClass = true;
        for (const auto& cls : pkg.classes) {
            if (!firstClass) fout << ",\n";
            firstClass = false;

            fout << "        {\n";
            fout << "          \"name\": \"" << cls.name << "\",\n";
            fout << "          \"stereotype\": \"" << cls.stereotype << "\",\n";
            fout << "          \"line\": " << cls.line << ",\n";
            fout << "          \"column\": " << cls.column << ",\n";

            // Pais (Parent Classes)
            fout << "          \"parent_classes\": [";
            for (size_t i = 0; i < cls.parentClasses.size(); ++i) {
                if (i > 0) fout << ", ";
                fout << "\"" << cls.parentClasses[i] << "\"";
            }
            fout << "],\n";

            // Atributos da Classe
            fout << "          \"attributes\": [\n";
            bool firstAttr = true;
            for (const auto& attr : cls.attributes) {
                if (!firstAttr) fout << ",\n";
                firstAttr = false;
                fout << "            { \"name\": \"" << attr.name << "\", \"type\": \"" << attr.type << "\", \"metaattribute\": \"" << attr.metaattribute << "\" }";
            }
            fout << "\n          ],\n";

            // Relações Internas da Classe
            fout << "          \"relations\": [\n";
            bool firstRel = true;
            for (const auto& rel : cls.relations) {
                if (!firstRel) fout << ",\n";
                firstRel = false;
                fout << "            {\n";
                fout << "              \"name\": \"" << rel.name << "\",\n";
                fout << "              \"stereotype\": \"" << rel.stereotype << "\",\n";
                fout << "              \"target_class\": \"" << rel.otherClass << "\",\n";
                fout << "              \"cardinality_source\": \"" << rel.cardinalitySource << "\",\n";
                fout << "              \"cardinality_target\": \"" << rel.cardinalityTarget << "\"\n";
                fout << "            }";
            }
            fout << "\n          ]\n";

            fout << "        }";
        }
        fout << "\n      ],\n"; // Fim Classes

        // --- 1.2 DATATYPES DO PACOTE ---
        fout << "      \"data_types\": [\n";
        bool firstDT = true;
        for (const auto& dt : pkg.dataTypes) {
            if (!firstDT) fout << ",\n";
            firstDT = false;
            fout << "        {\n";
            fout << "          \"name\": \"" << dt.name << "\",\n";
            fout << "          \"attributes\": [\n";
            bool firstDtAttr = true;
            for (const auto& attr : dt.attributes) {
                if (!firstDtAttr) fout << ",\n";
                firstDtAttr = false;
                fout << "            { \"name\": \"" << attr.name << "\", \"type\": \"" << attr.type << "\" }";
            }
            fout << "\n          ]\n";
            fout << "        }";
        }
        fout << "\n      ],\n"; // Fim DataTypes

        // --- 1.3 ENUMS DO PACOTE ---
        fout << "      \"enumerations\": [\n";
        bool firstEnum = true;
        for (const auto& en : pkg.enumerations) {
            if (!firstEnum) fout << ",\n";
            firstEnum = false;
            fout << "        {\n";
            fout << "          \"name\": \"" << en.name << "\",\n";
            fout << "          \"literals\": [";
            for (size_t i = 0; i < en.literals.size(); ++i) {
                if (i > 0) fout << ", ";
                fout << "\"" << en.literals[i] << "\"";
            }
            fout << "]\n";
            fout << "        }";
        }
        fout << "\n      ],\n"; // Fim Enums

        // --- 1.4 GENERALIZATIONS (GENSETS) DO PACOTE ---
        fout << "      \"generalizations\": [\n";
        bool firstGen = true;
        for (const auto& gen : pkg.generalizations) {
            if (!firstGen) fout << ",\n";
            firstGen = false;
            fout << "        {\n";
            fout << "          \"name\": \"" << gen.name << "\",\n";
            fout << "          \"restrictions\": \"" << gen.restrictions << "\",\n";
            fout << "          \"parent_class\": \"" << gen.parentClass << "\",\n";
            fout << "          \"child_classes\": [";
            for (size_t i = 0; i < gen.childClasses.size(); ++i) {
                if (i > 0) fout << ", ";
                fout << "\"" << gen.childClasses[i] << "\"";
            }
            fout << "]\n";
            fout << "        }";
        }
        fout << "\n      ],\n"; // Fim Generalizations

        // --- 1.5 RELAÇÕES EXTERNAS DO PACOTE ---
        fout << "      \"external_relations\": [\n";
        bool firstExtRel = true;
        for (const auto& extRel : pkg.externalRelations) {
            if (!firstExtRel) fout << ",\n";
            firstExtRel = false;
            fout << "        {\n";
            fout << "          \"name\": \"" << extRel.name << "\",\n";
            fout << "          \"stereotype\": \"" << extRel.stereotype << "\",\n";
            fout << "          \"source_class\": \"" << extRel.sourceClass << "\",\n";
            fout << "          \"target_class\": \"" << extRel.targetClass << "\",\n";
            fout << "          \"cardinality_source\": \"" << extRel.cardinalitySource << "\",\n";
            fout << "          \"cardinality_target\": \"" << extRel.cardinalityTarget << "\"\n";
            fout << "        }";
        }
        fout << "\n      ]\n"; // Fim External Relations

        fout << "    }"; // Fim do objeto Pacote
    }
    fout << "\n  ],\n"; // Fim do array Packages

    // --- 2. ERROS SINTÁTICOS/SEMÂNTICOS ---
    fout << "  \"errors\": [\n";
    const auto& errors = synthesisTable.getErrors();
    bool firstErr = true;
    for (const auto& err : errors) {
        if (!firstErr) fout << ",\n";
        firstErr = false;
        fout << "    {\n";
        fout << "      \"message\": \"" << err.message << "\",\n";
        fout << "      \"suggestion\": \"" << err.suggestion << "\",\n";
        fout << "      \"line\": " << err.line << ",\n";
        fout << "      \"column\": " << err.col << "\n";
        fout << "    }";
    }
    fout << "\n  ]\n";

    fout << "}\n"; // Fim do JSON
    fout.close();

    cout << YELLOW_TEXT << "Tabela de síntese exportada para " << filename << RESET_COLOR << endl;
}

