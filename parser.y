%{
#include <iostream>
#include <string>
#include <cstring>
#include "symbol_table.h"
#include "synthesis_table.h"
#include "file_generator.h"
#include "colors.h"
#include <vector>

using namespace std;


extern int yylex();
extern int yylineno;
extern int token_start_column; 
extern char* yytext;
extern FILE* yyin; 

void yyerror(const char *s);


SymbolTable symbolTable;
SynthesisTable synthesisTable;
FileGenerator fileGenerator(symbolTable, synthesisTable);

char* copyString(const char* s) {
    if (!s) return nullptr;
    char* d = new char[strlen(s) + 1];
    strcpy(d, s);
    return d;
}

%}

%union {
    char* sval; 
    vector<string>* svec;
}

%type <svec> image_classes
%type <sval> internal_relation_keyword
%type <sval> cardinality
%type <sval> cardinality_index
%type <sval> right_cardinality
%type <sval> generalization_restrictions
%type <sval> domain_classes
%type <sval> generalization_restriction_keyword


%token PALAVRA_RESERVADA

%token IMPORT
%token PACKAGE
%token GENSET
%token DISJOINT
%token COMPLETE
%token OVERLAPPING
%token INCOMPLETE
%token GENERAL
%token SPECIFICS
%token SPECIALIZES
%token WHERE
%token ENUM
%token RELATION
%token DATATYPE

%token <sval> DADO_NATIVO
%token <sval> META_ATRIBUTO
%token <sval> ESTERIOTIPO_CLASSE
%token <sval> ESTERIOTIPO_RELACAO
%token <sval> NOVO_TIPO
%token <sval> NOME_DE_CLASSE 
%token <sval> NOME_DE_RELACAO
%token <sval> INSTANCIA
%token <sval> NUMERO

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
%token <sval> ASTERISK

%token NAO_IDENTIFICADO

%%

ontology:
    import_declaration package_declaration body |
    package_declaration body 
    ;

package_declaration:
    PACKAGE NOME_DE_CLASSE { 
        synthesisTable.setCurrentPackage($2);
    }
    ;

import_declaration:
    IMPORT NOME_DE_CLASSE {
        // symbolTable.addImport($2, yylineno, token_start_column);
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
    class_header class_body_opt
;

class_header:
    ESTERIOTIPO_CLASSE NOME_DE_CLASSE
    {
        synthesisTable.addClass($2, $1, yylineno, token_start_column);
    }
  | ESTERIOTIPO_CLASSE NOME_DE_CLASSE SPECIALIZES image_classes
    {
        synthesisTable.addClassWithParents($2, $1, *$4, yylineno, token_start_column);
        delete $4;
    }
;

class_body_opt:
      /* empty */
    | LEFT_CURLY_BRACKETS class_body RIGHT_CURLY_BRACKETS
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
        synthesisTable.addAttribute($1, $3, "");
    } |

    NOME_DE_RELACAO COLON NOVO_TIPO {
        synthesisTable.addAttribute($1, $3, "");
    } |

    NOME_DE_RELACAO COLON DADO_NATIVO LEFT_CURLY_BRACKETS META_ATRIBUTO RIGHT_CURLY_BRACKETS {
        synthesisTable.addAttribute($1, $3, $5);
    } |

    NOME_DE_RELACAO COLON NOVO_TIPO LEFT_CURLY_BRACKETS META_ATRIBUTO RIGHT_CURLY_BRACKETS {
        synthesisTable.addAttribute($1, $3, $5);
    }
    ;

internal_relation_declaration:
    AT ESTERIOTIPO_RELACAO cardinality internal_relation_keyword cardinality NOME_DE_CLASSE {
        synthesisTable.addInternalRelationToCurrentClass($6, $5, $3, $4, $2);
    }
    ;

internal_relation_keyword:
    relation_keyword  {$$ = copyString("");} |
    relation_keyword NOME_DE_RELACAO GENERAL_RELATION_ARROW { $$ = $2; }
    ;

auxiliary_declaration:
    data_type_declaration |
    enum_declaration |
    external_relation_declaration |
    generalization_declaration
    ;

data_type_declaration:
    DATATYPE NOME_DE_CLASSE LEFT_CURLY_BRACKETS {
        synthesisTable.addDataType($2, yylineno, token_start_column);
    } attributes_declaration RIGHT_CURLY_BRACKETS;

enum_declaration:
    ENUM NOME_DE_CLASSE LEFT_CURLY_BRACKETS {
        synthesisTable.addEnumeration($2, yylineno, token_start_column);
    } enum_body RIGHT_CURLY_BRACKETS;

enum_body:
    enum_value |
    enum_value COMMA enum_body
    ;

enum_value:
    INSTANCIA {
        synthesisTable.addEnumValueToCurrentEnum($1);
    };

external_relation_declaration:
    AT ESTERIOTIPO_RELACAO RELATION NOME_DE_CLASSE cardinality internal_relation_keyword cardinality NOME_DE_CLASSE {
        synthesisTable.addExternalRelation($6, $2, $4, $5, $7, $8, yylineno, token_start_column);
    };

cardinality:
    LEFT_SQUARE_BRACKETS cardinality_index RIGHT_SQUARE_BRACKETS {
        $$ = $2;
    } | /* empty */ {
       $$ = copyString("?");
    }
    ;

cardinality_index:
    NUMERO {$$ = $1;} |
    NUMERO DOUBLEDOT right_cardinality {
        string s = string($1) + ".." + string($3);
        $$ = copyString(s.c_str());
    } 
    ;

right_cardinality:
    NUMERO {} {
        $$ = $1;
    } |
    ASTERISK {
        $$ = copyString("*");
    }
    ;

generalization_declaration:
    inline_generalization_declaration |
    block_generalization_declaration
    ;

inline_generalization_declaration:
    generalization_restrictions GENSET NOME_DE_CLASSE WHERE image_classes SPECIALIZES domain_classes {
        synthesisTable.addGeneralizationFull($3, $1, $7, *$5, yylineno, token_start_column);
        delete $5;
    }
    ;

generalization_restrictions:
    generalization_restriction_keyword {
        $$ = $1;
    } |
    generalization_restriction_keyword generalization_restrictions {
        string s = string($1) + " " + string($2);
        $$ = copyString(s.c_str());
    } | /* empty */ {
        $$ = copyString("");
    }
    ;

generalization_restriction_keyword:
    DISJOINT { $$ = copyString("disjoint"); } |
    COMPLETE { $$ = copyString("complete"); } |
    OVERLAPPING { $$ = copyString("overlapping"); } |
    INCOMPLETE { $$ = copyString("incomplete"); };

image_classes:
    NOME_DE_CLASSE {
        $$ = new std::vector<string>();
        $$->push_back($1);
    } |
    NOME_DE_CLASSE COMMA image_classes {
        $$ = $3;
        $$->insert($$->begin(), $1);
    }
    ;

domain_classes:
    NOME_DE_CLASSE {
        $$ = $1;
    };

block_generalization_declaration:
    generalization_restrictions GENSET NOME_DE_CLASSE LEFT_CURLY_BRACKETS {
        synthesisTable.startGeneralizationBlock($3, $1, yylineno, token_start_column);
    } generalization_body RIGHT_CURLY_BRACKETS;

generalization_body:
    generalization_body_domain generalization_body_image 
    ;

generalization_body_domain:
    GENERAL NOME_DE_CLASSE {
        synthesisTable.setGeneralizationParent($2);
    }
    ;

generalization_body_image:
    SPECIFICS image_classes {
        synthesisTable.addGeneralizationChildren(*$2);
        delete $2;
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
    fileGenerator.generateSynthesisTableJson("output/synthesis_table.json");

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