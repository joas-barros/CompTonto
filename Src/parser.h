#include <FlexLexer.h>

#ifndef PARSER_H
#define PARSER_H

class Parser {
    private:
        yyFlexLexer lexer;
        int lookahead;
        void generateSummary();
        void exportJson();
    public:
        void start();
}

#endif