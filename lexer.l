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

esteriotipo_classe event|situation|process|category|mixin|phaseMixin|roleMixin|historicalMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole|relator|type

esteriotipo_relacao material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution|subCategoryOf

palavras_reservadas functional-complexes|intrinsic-modes

import import

package package

disjoint disjoint

complete complete

general general

overlapping overlapping

incomplete incomplete

specializes specializes

specifics specifics

where where

genset genset

enum enum

relation relation

datatype datatype

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

{import}            {return processToken(IMPORT);}

{package}           {return processToken(PACKAGE);}

{disjoint}          {return processToken(DISJOINT);}

{complete}          {return processToken(COMPLETE);}

{general}           {return processToken(GENERAL);}

{overlapping}       {return processToken(OVERLAPPING);}

{incomplete}        {return processToken(INCOMPLETE);}

{specializes}       {return processToken(SPECIALIZES);}

{specifics}         {return processToken(SPECIFICS);}

{where}             {return processToken(WHERE);}

{genset}            {return processToken(GENSET);}

{enum}              {return processToken(ENUM);}

{relation}         {return processToken(RELATION);}

{datatype}          {return processToken(DATATYPE);}

{dados_nativos}     { yylval.sval = strdup(yytext); return processToken(DADO_NATIVO);}
    
{meta_atributos}    { yylval.sval = strdup(yytext); return processToken(META_ATRIBUTO);}

{esteriotipo_classe} { yylval.sval = strdup(yytext); return processToken(ESTERIOTIPO_CLASSE);}

{esteriotipo_relacao} { yylval.sval = strdup(yytext); return processToken(ESTERIOTIPO_RELACAO);}

{novos_tipos}       { yylval.sval = strdup(yytext); return processToken(NOVO_TIPO);}

{nome_de_classe}    {
                        yylval.sval = strdup(yytext); 
                        return processToken(NOME_DE_CLASSE);
                    }

{nome_de_relacao}   {
                        yylval.sval = strdup(yytext);
                        return processToken(NOME_DE_RELACAO);
                    }

{instancias}        { yylval.sval = strdup(yytext); return processToken(INSTANCIA);}

{number}            { yylval.sval = strdup(yytext);
                    return processToken(NUMERO);}

"{"        { return processToken(LEFT_CURLY_BRACKETS); }
"}"        { return processToken(RIGHT_CURLY_BRACKETS); }
"("        { return processToken(LEFT_PARENTHESIS); }
")"        { return processToken(RIGHT_PARENTHESIS); }
"["        { return processToken(LEFT_SQUARE_BRACKETS); }
"]"        { return processToken(RIGHT_SQUARE_BRACKETS); }
".."       { return processToken(DOUBLEDOT); }

"<>--"      { return processToken(LEFT_AGGREGATION_ARROW); }
"--<>"      { return processToken(RIGHT_AGGREGATION_ARROW); }
"--<o>"     { return processToken(RIGHT_COMPOSITION_ARROW); }
"<o>--"     { return processToken(LEFT_COMPOSITION_ARROW); }
"--"        { return processToken(GENERAL_RELATION_ARROW); }

"*"        { yylval.sval = strdup("*"); return processToken(ASTERISK); }
"@"        { return processToken(AT); }
":"        { return processToken(COLON); }
","        { return processToken(COMMA); }

.                   { 
                      token_start_column = yycolumn; 
                      cerr << RED_TEXT << "Erro Léxico: Símbolo não identificado '" << yytext << "' na linha " << yylineno << ", coluna " << token_start_column << RESET_COLOR << endl;
                      yycolumn += yyleng;
                      return NAO_IDENTIFICADO;
                   }
%%