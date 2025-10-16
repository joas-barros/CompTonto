%{
#include <iostream>
#include <fstream>
#include "tokens.h"
#include "symbol_table.h"


using namespace std;

int cont_classes = 0, cont_relacoes = 0, cont_palavras_reservadas = 0, cont_individuos = 0, cont_palavras_chave = 0, cont_meta_atributos = 0;

SymbolTable symbolTable;
ofstream fout;

void generateReport();

%}

%option noyywrap
%option yylineno

delim [ \t\n]
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

{ws}                { /* ignore whitespace */ }

{number}             {
                      symbolTable.addSymbol(yytext, NUMERO, yylineno);
                      //return NUMERO; 
                     }

{palavras_reservadas} {
                        cont_palavras_reservadas++;
                      symbolTable.addSymbol(yytext, PALAVRA_RESERVADA, yylineno);
                      //return PALAVRA_RESERVADA;
                     }

{dados_nativos}     {
                      symbolTable.addSymbol(yytext, DADO_NATIVO, yylineno);
                      //return DADO_NATIVO;
                    }

{meta_atributos}    {
                        cont_meta_atributos++;
                      symbolTable.addSymbol(yytext, META_ATRIBUTO, yylineno);
                      //return META_ATRIBUTO;
                    }

{esteriotipo_classe} {
                      symbolTable.addSymbol(yytext, ESTERIOTIPO_CLASSE, yylineno);
                      //return ESTERIOTIPO_CLASSE;
                    }

{esteriotipo_relacao} {
                      symbolTable.addSymbol(yytext, ESTERIOTIPO_RELACAO, yylineno);
                      //return ESTERIOTIPO_RELACAO;
                    }

{novos_tipos}       {
                      symbolTable.addSymbol(yytext, NOVO_TIPO, yylineno);
                      //return NOVO_TIPO;
                    }

{nome_de_classe}    {
                      cont_classes++;
                      symbolTable.addSymbol(yytext, NOME_DE_CLASSE, yylineno);
                      //return NOME_DE_CLASSE;
                    }

{nome_de_relacao}   {
                      cont_relacoes++;
                      symbolTable.addSymbol(yytext, NOME_DE_RELACAO, yylineno);
                      //return NOME_DE_RELACAO;
                    }

{instancias}        {
                        cont_individuos++;
                      symbolTable.addSymbol(yytext, INSTANCIA, yylineno);
                      //return INSTANCIA;
                    }

"{"                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"}"                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"("                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"["                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"]"                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }
".."               {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"<>--"             {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"--<>"             {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"*"                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"@"                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

":"                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

","                {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

"--"              {
                      symbolTable.addSymbol(yytext, SIMBOLO, yylineno);
                      //return SIMBOLO;
                    }

.                   { 
                      cerr << "ERRO: Caracter desconhecido " << yytext << " na linha " << yylineno << endl; 
                      symbolTable.addSymbol(yytext, NAO_IDENTIFICADO, yylineno);
                      //return NAO_IDENTIFICADO;
                   }
%%

int main() {
   yyFlexLexer lexer;
   lexer.yylex();

   generateReport();
   return 0;
}

void generateReport() {
   fout.open("output/relatorio.txt");

   if (!fout.is_open()) {
       cerr << "Erro ao abrir o arquivo de relatório." << endl;
       return;
   }

   fout << "RELATÓRIO DE ANÁLISE LÉXICA" << endl;
   fout << "==========================" << endl;
   fout << "Número de classes: " << cont_classes << endl;
   fout << "Número de relações: " << cont_relacoes << endl;
   fout << "Número de palavras reservadas: " << cont_palavras_reservadas << endl;
   fout << "Número de indivíduos: " << cont_individuos << endl;
   fout << "Número de palavras-chave: " << cont_palavras_chave << endl;
   fout << "Número de meta-atributos: " << cont_meta_atributos << endl;
   fout << "==========================" << endl;
   fout.close();

   cout << "======================ANÁLISE LEXICA CONCLUIDA COM SUCESSO!======================" << endl;
   cout << "Relatório gerado em output/relatorio.txt" << endl;

    symbolTable.exportJson();
}