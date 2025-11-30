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
%token RELATION
%token DATATYPE

%token DADO_NATIVO
%token META_ATRIBUTO
%token ESTERIOTIPO_CLASSE
%token ESTERIOTIPO_RELACAO
%token NOVO_TIPO
%token <sval> NOME_DE_CLASSE 
%token <sval> NOME_DE_RELACAO
%token INSTANCIA
%token NUMERO

%token RIGHT_PARENTHESIS
%token LEFT_PARENTHESIS
%token RIGHT_CURLY_BRACKETS
%token LEFT_CURLY_BRACKETS
%token RIGHT_SQUARE_BRACKETS
%token LEFT_SQUARE_BRACKETS
%token COMMA
%token AT
%token RELATION_KEYWORD
%token COLON
%token DOUBLEDOT
%token ASTERISK

%token NAO_IDENTIFICADO

%%

/* programa:
    statements
    ;

statements:
    statements statement
    | statement
    ;

statement:
    PALAVRA_RESERVADA
    | IMPORT
    | PACKAGE
    | GENSET
    | DISJOINT
    | COMPLETE
    | GENERAL
    | SPECIFICS
    | SPECIALIZES
    | WHERE
    | ENUM
    | RELATION
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
    | RIGHT_PARENTHESIS
    | LEFT_PARENTHESIS
    | RIGHT_CURLY_BRACKETS
    | LEFT_CURLY_BRACKETS
    | RIGHT_SQUARE_BRACKETS
    | LEFT_SQUARE_BRACKETS
    | COMMA
    | AT
    | RELATION_KEYWORD
    | COLON
    | DOUBLEDOT
    | ASTERISK
    ; */

ontology:
    package_declaration body
    ;

package_declaration:
    PACKAGE NOME_DE_CLASSE { 
        // symbolTable.setCurrentPackage($2);
    }
    ;

body:
    statements |
    statements body
    ;

statements:
    class_declaration |
    auxiliary_declaration
    ;

class_declaration:
    ESTERIOTIPO_CLASSE NOME_DE_CLASSE {
        // symbolTable.addClass($2, $1, yylineno, token_start_column);
    } |
    ESTERIOTIPO_CLASSE NOME_DE_CLASSE LEFT_CURLY_BRACKETS class_body RIGHT_CURLY_BRACKETS {
        // symbolTable.addClass($2, $1, yylineno, token_start_column);
    }
    ;

class_body:
    class_body_element |
    class_body_element class_body
    ;

class_body_element:
    attributes_declaration class_body_element |
    internal_relation_declaration class_body_element |
    attributes_declaration |
    internal_relation_declaration
    ;

attributes_declaration:
    attribute_declaration |
    attribute_declaration attributes_declaration
    ;

attribute_declaration:
    NOME_DE_RELACAO COLON DADO_NATIVO {
        // symbolTable.addAttributeToCurrentClass($1, $3, "", yylineno, token_start_column);
    } |

    NOME_DE_RELACAO COLON NOVO_TIPO {
        // symbolTable.addAttributeToCurrentClass($1, $3, "", yylineno, token_start_column);
    } |

    NOME_DE_RELACAO COLON DADO_NATIVO LEFT_CURLY_BRACKETS META_ATRIBUTO RIGHT_CURLY_BRACKETS {
        // symbolTable.addAttributeToCurrentClass($1, $3, $5, yylineno, token_start_column);
    } |

    NOME_DE_RELACAO COLON NOVO_TIPO LEFT_CURLY_BRACKETS META_ATRIBUTO RIGHT_CURLY_BRACKETS {
        // symbolTable.addAttributeToCurrentClass($1, $3, $5, yylineno, token_start_column);
    }
    ;

internal_relation_declaration:
    AT ESTERIOTIPO_RELACAO cardinality RELATION_KEYWORD cardinality NOME_DE_CLASSE {
        // symbolTable.addInternalRelationToCurrentClass($3, $4, $6, $2, yylineno, token_start_column);
    }
    ;

auxiliary_declaration:
    data_type_declaration |
    enum_declaration |
    external_relation_declaration |
    generalization_declaration
    ;

data_type_declaration:
    DATATYPE NOME_DE_CLASSE LEFT_CURLY_BRACKETS attributes_declaration RIGHT_CURLY_BRACKETS {
        // symbolTable.addDataType($2, yylineno, token_start_column);
    };

enum_declaration:
    ENUM NOME_DE_CLASSE LEFT_CURLY_BRACKETS enum_body RIGHT_CURLY_BRACKETS {
        // symbolTable.addEnum($2, yylineno, token_start_column);
    }
    ;

enum_body:
    enum_value |
    enum_value COMMA enum_body
    ;

enum_value:
    INSTANCIA {
        // symbolTable.addEnumValueToCurrentEnum($1, yylineno, token_start_column);
    };

external_relation_declaration:
    AT ESTERIOTIPO_RELACAO NOME_DE_RELACAO NOME_DE_CLASSE cardinality RELATION_KEYWORD cardinality NOME_DE_CLASSE {
        // symbolTable.addExternalRelation($3, $2, $4, $5, $7, yylineno, token_start_column);
    };

cardinality:
    LEFT_SQUARE_BRACKETS cardinality_index RIGHT_SQUARE_BRACKETS {
        // $$ = $2;
    } 
    ;

cardinality_index:
    NUMERO |
    NUMERO DOUBLEDOT right_cardinality
    ;

right_cardinality:
    NUMERO |
    ASTERISK
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