#ifndef PARSER_H
#define PARSER_H

#define yyFlexLexer MyLexer
#include <FlexLexer.h>
#include "symbol_table.h"
#include "tokens.h"

class Parser {
    private:
        MyLexer lexer;
        int lookahead;
        SymbolTable symbolTable;

        void generateSummary();
        void exportJson();
    public:
        void start();
}

#endif