%{
#include <iostream>
#include <fstream>
#include "tokens.h"
#include "symbol_table.h"
#include "colors.h"


using namespace std;

// Contadores para o relatório
int cont_classes = 0, cont_relacoes = 0, cont_palavras_reservadas = 0, cont_individuos = 0, cont_palavras_chave = 0, cont_meta_atributos = 0;

// Variáveis para rastreamento de posição
int yycolumn = 1; 
int token_start_column = 1; 

// Tabela de símbolos
SymbolTable symbolTable;
ofstream fout;

void generateReport();

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

esteriotipo_classe event|situation|process|category|mixin|phaseMixin|roleMixin|historicalMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole

esteriotipo_relacao material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution

palavras_reservadas genset|disjoint|complete|general|specifies|where|package|import|functional-complexes|intrinsic-modes

dados_nativos number|string|boolean|date|time|datetime

meta_atributos ordered|const|derived|subsets|redefines

nome_de_classe {upper}(({letter}|_)*{letter})?

nome_de_relacao {lower}(({letter}|_)*{letter})?

instancias ^{letter}[a-zA-Z_]*{digit}+$

novos_tipos {letter}+DataType

%%

{ws}                { yycolumn += yyleng; }

\n                  { yycolumn = 1; }

{number}            {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return NUMERO;
                    }

{palavras_reservadas} {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return PALAVRA_RESERVADA;

                        }

{dados_nativos}     {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return DADO_NATIVO;
                    }

{meta_atributos}    {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return META_ATRIBUTO;
                    }

{esteriotipo_classe} {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return ESTERIOTIPO_CLASSE;
                    }

{esteriotipo_relacao} {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return ESTERIOTIPO_RELACAO;
                    }

{novos_tipos}       {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return NOVO_TIPO;
                    }

{nome_de_classe}    {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return NOME_DE_CLASSE;
                    }

{nome_de_relacao}   {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return NOME_DE_RELACAO;
                    }

{instancias}        {
                        token_start_column = yycolumn;
                        yycolumn += yyleng;
                        return INSTANCIA;
                    }

"{"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"}"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"("                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

")"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"["                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"]"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

".."               {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"<>--"             {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"--<>"             {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"*"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"@"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

":"                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

","                {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

"--"               {
                    token_start_column = yycolumn;
                    yycolumn += yyleng;
                    return SIMBOLO;
                    }

.                   { 
                      token_start_column = yycolumn; yycolumn += yyleng;
                      cerr << RED_TEXT << "ERRO: Caracter desconhecido " << yytext << " na linha " << yylineno << RESET_COLOR << endl; 
                      return NAO_IDENTIFICADO;
                   }
%%

int main() {
   yyFlexLexer lexer;
   int lookahead;

   while ((lookahead = lexer.yylex()) != 0) {
        symbolTable.addSymbol(lexer.YYText(), lookahead, lexer.lineno(), token_start_column);
   }

   generateReport();
   return 0;
}

void generateReport() {
    symbolTable.exportJson();
    symbolTable.generateReport();

    cout << BLUE_TEXT << "===========================================" << RESET_COLOR << endl;
    cout << GREEN_TEXT << "ANÁLISE LÉXICA CONCLUÍDA COM SUCESSO! \\O/" << RESET_COLOR << endl;
    cout << BLUE_TEXT << "===========================================" << RESET_COLOR << endl;
}