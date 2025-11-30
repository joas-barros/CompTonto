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
extern FILE* yyin; 

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

%token LEFT_COMPOSITION_ARROW
%token RIGHT_COMPOSITION_ARROW
%token LEFT_AGGREGATION_ARROW
%token RIGHT_AGGREGATION_ARROW
%token GENERAL_RELATION_ARROW

%token COLON
%token DOUBLEDOT
%token ASTERISK

%token NAO_IDENTIFICADO

%%

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
    } |
    ESTERIOTIPO_CLASSE NOME_DE_CLASSE SPECIALIZES image_classes {
    }
    ;

class_body:
    class_body_element |
    class_body_element class_body
    ;

class_body_element:
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
    AT ESTERIOTIPO_RELACAO cardinality internal_relation_keyword cardinality NOME_DE_CLASSE {
        // symbolTable.addInternalRelationToCurrentClass($3, $4, $6, $2, yylineno, token_start_column);
    }
    ;

internal_relation_keyword:
    relation_keyword |
    relation_keyword NOME_DE_RELACAO GENERAL_RELATION_ARROW
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
    AT ESTERIOTIPO_RELACAO RELATION NOME_DE_RELACAO NOME_DE_CLASSE cardinality relation_keyword cardinality NOME_DE_CLASSE {
        // symbolTable.addExternalRelation($3, $2, $4, $5, $7, yylineno, token_start_column);
    };

cardinality:
    LEFT_SQUARE_BRACKETS cardinality_index RIGHT_SQUARE_BRACKETS {
    } |
    /* empty */

    ;

cardinality_index:
    NUMERO |
    NUMERO DOUBLEDOT right_cardinality
    ;

right_cardinality:
    NUMERO |
    ASTERISK
    ;

generalization_declaration:
    inline_generalization_declaration |
    block_generalization_declaration
    ;

inline_generalization_declaration:
    generalization_restrictions GENSET NOME_DE_CLASSE WHERE image_classes SPECIALIZES domain_classes {
        // symbolTable.addGeneralization($3, $1, $5, $7, yylineno, token_start_column);
    }
    ;

generalization_restrictions:
    DISJOINT |
    COMPLETE |
    DISJOINT COMPLETE |
    /* empty */
    ;

image_classes:
    NOME_DE_CLASSE |
    NOME_DE_CLASSE COMMA image_classes
    ;

domain_classes:
    NOME_DE_CLASSE;

block_generalization_declaration:
    generalization_restrictions GENSET NOME_DE_CLASSE LEFT_CURLY_BRACKETS generalization_body RIGHT_CURLY_BRACKETS {
        // symbolTable.addGeneralization($2, "", "", "", yylineno, token_start_column);
    }
    ;

generalization_body:
    generalization_body_domain generalization_body_image 
    ;

generalization_body_domain:
    GENERAL NOME_DE_CLASSE {
        // symbolTable.addGeneralizationDomain($2, yylineno, token_start_column);
    }
    ;

generalization_body_image:
    SPECIFICS image_classes {
        // symbolTable.addGeneralizationImage($2, yylineno, token_start_column);
    }
    ;

relation_keyword:
    LEFT_COMPOSITION_ARROW |
    RIGHT_COMPOSITION_ARROW |
    LEFT_AGGREGATION_ARROW |
    RIGHT_AGGREGATION_ARROW |
    GENERAL_RELATION_ARROW
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


int main(int argc, char** argv) {
    // 1. Verificação de argumentos
    if (argc < 2) {
        cerr << RED_TEXT << "Erro: Nenhum arquivo de entrada fornecido." << RESET_COLOR << endl;
        cerr << "Uso: ./comptonto <caminho_do_arquivo>" << endl;
        return 1;
    }

    // 2. Tentar abrir o arquivo
    FILE* arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        cerr << RED_TEXT << "Erro: Não foi possível abrir o arquivo '" << argv[1] << "'." << RESET_COLOR << endl;
        return 1;
    }

    // 3. Direcionar o Flex para ler deste arquivo
    yyin = arquivo;

    // 4. Executar o Parser e capturar o resultado
    // yyparse retorna 0 em sucesso, 1 em caso de erro sintático
    int resultado = yyparse();

    // 5. Fechar o arquivo
    fclose(arquivo);

    // 6. Verificar validade
    if (resultado == 0) {
        // Se chegou aqui, a gramática aceitou o arquivo inteiro
        cout << GREEN_TEXT << "\n[SUCESSO] O arquivo '" << argv[1] << "' é VÁLIDO segundo a gramática!" << RESET_COLOR << endl;
        generateReport(); // Gera os relatórios apenas se for válido (opcional)
    } else {
        // Se houve erro, o yyerror já imprimiu a mensagem de erro específica
        cout << RED_TEXT << "\n[FALHA] O arquivo '" << argv[1] << "' contém erros sintáticos e é INVÁLIDO." << RESET_COLOR << endl;
        // Opcional: Você pode querer gerar o relatório de erros léxicos mesmo assim
        // fileGenerator.generateErrorReport("output/error_report.txt");
    }

    return resultado;
}