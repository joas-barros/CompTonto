%{
#include <iostream>
#include "symbol_table.h"
#include "colors.h"
#include "parser.hpp"


using namespace std;

// Variáveis para rastreamento de posição
int yycolumn = 1; 
int token_start_column = 1; 

// Tabela de símbolos
extern SymbolTable symbolTable;

int processToken(int tokenType) {
    token_start_column = yycolumn;
    yycolumn += yyleng;
    
    
    symbolTable.addSymbol(yytext, tokenType, yylineno, token_start_column);
    
    return tokenType;
}

%}

%option noyywrap
%option yylineno

delim [ \t]
ws {delim}+
digit [0-9]
letter [a-zA-Z]
upper [A-Z]
lower [a-z]
number {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

esteriotipo_classe event|situation|process|category|mixin|phaseMixin|roleMixin|historicalMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole|relator

esteriotipo_relacao material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution

palavras_reservadas genset|disjoint|complete|general|specifies|where|package|import|functional-complexes|intrinsic-modes

dados_nativos number|string|boolean|date|time|datetime

meta_atributos ordered|const|derived|subsets|redefines

nome_de_classe {upper}(({letter}|_)*{letter}?)?

nome_de_relacao {lower}(({letter}|_)*{letter}?)?

instancias ^{letter}[a-zA-Z_]*{digit}+$

novos_tipos {letter}+DataType

%%

{ws}                { yycolumn += yyleng; }

\n                  { yycolumn = 1; }

{palavras_reservadas} {return processToken(PALAVRA_RESERVADA);}

{dados_nativos}     {return processToken(DADO_NATIVO);}

{meta_atributos}    {return processToken(META_ATRIBUTO);}

{esteriotipo_classe} {return processToken(ESTERIOTIPO_CLASSE);}

{esteriotipo_relacao} {return processToken(ESTERIOTIPO_RELACAO);}

{novos_tipos}       {return processToken(NOVO_TIPO);}

{nome_de_classe}    {
                        yylval.sval = strdup(yytext); 
                        return processToken(NOME_DE_CLASSE);
                    }

{nome_de_relacao}   {
                        yylval.sval = strdup(yytext);
                        return processToken(NOME_DE_RELACAO);
                    }

{instancias}        {return processToken(INSTANCIA);}

{number}            {return processToken(NUMERO);}

"{"         |
"}"         |
"("         |
")"         |
"["         |
"]"         |
".."        |
"<>--"      |
"--<>"      |
"*"         |
"@"         |
":"         |
","         |
"--"        { return processToken(SIMBOLO); }

.                   { 
                      token_start_column = yycolumn; 
                      cerr << RED_TEXT << "Erro Léxico: Símbolo não identificado '" << yytext << "' na linha " << yylineno << ", coluna " << token_start_column << RESET_COLOR << endl;
                      yycolumn += yyleng;
                      return NAO_IDENTIFICADO;
                   }
%%