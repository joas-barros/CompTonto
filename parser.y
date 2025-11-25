%{
#include <iostream>
#include <string>
#include "symbol_table.h"
#include "file_generator.h"
#include "colors.h"

using namespace std;


extern int yylex();
extern int yylineno;
extern int token_start_column; 
extern char* yytext;

void yyerror(const char *s);


SymbolTable symbolTable;
FileGenerator fileGenerator(symbolTable);

%}

%union {
    char* sval; 
    int ival;   
}

%token PALAVRA_RESERVADA
%token DADO_NATIVO
%token META_ATRIBUTO
%token ESTERIOTIPO_CLASSE
%token ESTERIOTIPO_RELACAO
%token NOVO_TIPO
%token <sval> NOME_DE_CLASSE 
%token <sval> NOME_DE_RELACAO
%token INSTANCIA
%token NUMERO
%token SIMBOLO
%token NAO_IDENTIFICADO

%%

programa:
    statements
    ;

statements:
    statements statement
    | statement
    ;

statement:
    PALAVRA_RESERVADA
    | DADO_NATIVO
    | META_ATRIBUTO
    | ESTERIOTIPO_CLASSE
    | ESTERIOTIPO_RELACAO
    | NOVO_TIPO
    | NOME_DE_CLASSE { 
        cout << "Parser reconheceu uma classe: " << $1 << endl;
    }
    | NOME_DE_RELACAO
    | INSTANCIA
    | NUMERO
    | SIMBOLO
    ;

%%

void generateReport() {
    fileGenerator.generateSymbolTableJson("output/symbol_table.json");
    fileGenerator.generateSymbolReport("output/symbol_report.txt");
    fileGenerator.generateErrorReport("output/error_report.txt");

    cout << BLUE_TEXT << "===========================================" << RESET_COLOR << endl;
    cout << GREEN_TEXT << "ANÁLISE SINTÁTICA CONCLUÍDA! \\O/" << RESET_COLOR << endl;
    cout << BLUE_TEXT << "===========================================" << RESET_COLOR << endl;
}

void yyerror(const char *s) {
    cerr << RED_TEXT << "Erro de Sintaxe na linha " << yylineno 
         << " coluna " << token_start_column << ": " << s << RESET_COLOR << endl;
}


int main() {
    yyparse();
    
    generateReport();
    return 0;
}