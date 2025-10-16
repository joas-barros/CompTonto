#include "parser.h"

void Parser::start() {
    lookahead = lexer.yylex();
    while (lookahead != 0) { // 0 indica EOF
        string name = lexer.YYText();
        int line = lexer.lineno();
        symbolTable.addSymbol(name, lookahead, line);
        lookahead = lexer.yylex();
    }
    generateSummary();
    exportJson();
}

void Parser::generateSummary() {
    // Implementar a geração do resumo
}

void Parser::exportJson() {
    // Implementar a exportação para JSON
}
