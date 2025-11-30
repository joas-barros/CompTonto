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

%token IMPORT
%token PACKAGE
%token GENSET
%token DISJOINT
%token COMPLETE
%token GENERAL
%token SPECIFICS
%token SPECIALIZES
%token WHERE
%token ENUM

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

%token RIGHT_BRACES
%token LEFT_BRACES
%token RIGHT_CURLY_BRACKETS
%token LEFT_CURLY_BRACKETS
%token RIGHT_SQUARE_BRACKETS
%token LEFT_SQUARE_BRACKETS
%token COMMA
%token AT
%token RELATION_KEYWORD
%token COLON
%token TWO_DOTS
%token ASTERISK

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
    | NOME_DE_RELACAO { 
        cout << "Parser reconheceu uma relação: " << $1 << endl;
    }
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