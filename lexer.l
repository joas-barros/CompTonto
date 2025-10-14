%{
#include <iostream>
#include "token.h"

using namespace std;
%}

%option noyywrap
%option yylineo

delim [ \t\n]
ws {delim}+
digit [0-9]
letter [a-zA-Z]
upper [A-Z]
lower [a-z]
number {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

esteriotipo_classe event|situation|process|category|mixin|phaseMixin|roleMixin|historicalMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole

esteriotipo_relacao material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution

palavras_reservadas genset|disjoint|complete|general|specifies|where|package

dados_nativos number|string|boolean|date|time|datetime

meta_atributos ordered|const|derived|subsets|redefines

nome_de_classe {upper}(({letter}|_)*{letter})?

nome_de_relacao {lower}(({letter}|_)*{letter})?

instancias ^{letter}[a-zA-Z_]*{digit}+$

novos_tipos {letter}+DataType

%%

{ws}                { /* ignore whitespace */ }

{number}            return NUMERO;

{palavras_reservadas} return PALAVRA_RESERVADA;

{dados_nativos}     return DADO_NATIVO;

{meta_atributos}    return META_ATRIBUTO;

{esteriotipo_classe} return ESTERIOTIPO_CLASSE;

{esteriotipo_relacao} return ESTERIOTIPO_RELACAO;

{novos_tipos}       return NOVO_TIPO;

{nome_de_classe}    return NOME_DE_CLASSE;

{nome_de_relacao}   return NOME_DE_RELACAO;

{instancias}        return INSTANCIA;

"{"                return SIMBOLO;
"}"                return SIMBOLO;
"("                return SIMBOLO;
"["                return SIMBOLO;
"]"                return SIMBOLO;
".."               return SIMBOLO;
"<>--"             return SIMBOLO;
"--<>"             return SIMBOLO;
"*"                return SIMBOLO;
"@"                return SIMBOLO;
":"                return SIMBOLO;

.                   { 
                      cerr << "ERRO: Caracter desconhecido " << yytext << " na linha " << yylineno << endl; 
                      return NAO_IDENTIFICADO;
                   }
%%