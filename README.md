# CompTonto

Este repositório tem como finalidade hospedar a implementação, juntamente com a documentação, de um compilador para a linguagem TONTO sendo o foco do momento no analisador léxico

## Sumário

* [Objetivos](#objetivos)
* [Tecnologias utilizadas](#tecnologias-utilizadas)
* [Tutorial de execução](#tutorial-de-execução)
  * [Instalação das dependências](#instalação-das-dependências)
  * [Baixar repositório do github](#baixar-repositório-do-github)
  * [Build](#build)
  * [Execução](#execução)
* [Estrutura do projeto](#estrutura-do-projeto)
* [Resultados gerados pelo analisador](#resultados-gerados-pelo-analisador)
  * [Tabela de símbolos](#tabela-de-símbolos)
  * [Tokens identificados](#tokens-identificados)
  * [Relatório de análise léxica](#relatório-de-análise-léxica)
* [Tratamento de exceções](#tratamento-de-exceções)
* [Próximos passos](#próximos-passos)
* [Referências](#referências)

## Objetivos

TONTO é o acrônimo para **Textual Ontology Language**, uma linguagem projetada para especificar textualmente ontologias como artefatos computacionais.

O objetivo central deste trabalho é a implementação de um **analisador léxico** para a linguagem TONTO. Esta ferramenta é a primeira etapa no processo de compilação, sendo responsável por processar o código-fonte e reconhecê-lo como uma sequência de unidades léxicas (ou *tokens*).

Os principais elementos textuais que o analisador identifica incluem:

* `Estereótipos de classe`
* `Estereótipos de relações`
* `Palavras reservadas`
* `Símbolos especiais`
* `Convenção para nomes de classes`
* `Convenção para nomes de relações`
* `Convenção para nomes de instâncias`
* `Tipos de dados nativos`
* `Novos tipos`
* `Meta-atributos`

A implementação deste analisador cumpre dois propósitos principais:

1.  **Validação Léxica:** Realizar a validação inicial do código-fonte, garantindo que todos os símbolos, palavras-chave e convenções de nomenclatura utilizados são válidos dentro da especificação da linguagem TONTO.
2.  **Métricas de Código:** Permitir a visualização da contagem de *tokens* específicos presentes no código. Isso oferece uma ideia inicial sobre a complexidade e o escopo da ontologia projetada (ex: número de classes, relações, atributos).

## Tecnologias utilizadas

- **Flex**: (Ferramenta de análise léxica)
- **C++17**: Linguagem de programação e versão utilizada
- **CMake**: Ferrmanta para gerar os arquivos de build (Versão 3.0 ou superior)
- **G++**: Compilador da linguagem C++ para linux
- **GBD**: Ferramenta de depuração
- **Visual Studio Code**: Editor de código 
- **Git** - Versionamento do código

## Tutorial de execução

### Instalação das dependências

Caso não possua todas as dependências necessárias para executar o projeto, digite os seguintes comandos no seu prompt de acordo com a distro do linux:

```bash
# Ubuntu/Debian
sudo apt-get install flex cmake build-essential

# Fedora/RHEL
sudo dnf install flex cmake gcc-c++
```

### Baixar repositório do github

Abra o terminal a partir do diretorio em que deseja alocar o projeto e digite os seguintes comandos:

```bash
# Clonar repositorio na máquina local
git clone https://github.com/joas-barros/CompTonto.git

# Navegar para o diretório do projeto
cd CompTonto

# Abrir o VS Code
code 
```

### Build

Para gerar o executável do projeto, certifique-se de que está no diretório raiz do projeto e siga os seguintes passos no terminal 

```bash
# 1. Acesse o diretório de build
cd Build

# 2. Execute o CMake para configurar o projeto
cmake ..

# 3. Compile o projeto
cmake --build .
```

### Execução

Para executar o projeto, siga os seguintes comandos no terminal:

```bash
# 1. Voltar ao diretorio raiz do projeto
cd ..

# 2. Execute o aquivo passando o conteudo de teste.tonto como argumaneto
 Build/comptonto < ./teste.tonto
```

O arquivo `teste.tonto` é o arquivo da analise léxica, caso queira mudar o exemplo, basta copiar o conteúdo para dentro de `teste.tonto` e seguir os passos anteriores.

## Estrutura do projeto

```bash
COMPTONTO
├── .vscode/                 # Configurações do Visual Studio Code
├── Build/                   # Diretório de build 
├── include/                 # Cabeçalhos do projeto (arquivos .h)
│   ├── colors.h             # Definições de cores 
│   ├── file_generator.h     # Interface para geração de arquivos 
│   ├── symbol_table.h       # Declarações da tabela de símbolos
│   └── tokens.h             # enums de tokens do analisador léxico
├── output/                  # Saídas geradas pelo analisador
├── src/                     # Implementações principais (arquivos .cpp)
│   ├── file_generator.cpp   # Implementação da geração de arquivos de saída
│   └── symbol_table.cpp     # Implementação da tabela de símbolos
├── .gitignore               # Arquivo para ignorar arquivos/pastas no Git
├── CMakeLists.txt           # Configuração do build system CMake
├── lexer.l                  # Arquivo Flex para o analisador léxico
├── README.md                # Documentação principal do projeto
└── teste.tonto              # Arquivo de teste de entrada 
```


## Resultados gerados pelo analisador

Ao fim do processo de análise léxica, são gerados 3 arquivos no diretório `outputs/`, sendo cada um voltado para uma métrica específica da análise.

### Tabela de símbolos

O primeiro arquivo a ser gerado é o `symbol_table.json`, nele, estão contidos todos os lexemas encontrados pelo analisador, bem como a qual token ele pertence, e uma lista com a localização de todas as suas ocorrências no código.

```json
{
  "symbols": [
    {
      "name": "import",
      "token": "PALAVRA_RESERVADA",
      "locations": [
        {"line": 1, "col": 1}
      ]
    },
    {
      "name": "Medicine",
      "token": "NOME_DE_CLASSE",
      "locations": [
        {"line": 25, "col": 38},
        {"line": 27, "col": 40}
      ]
    }
  ]
}
```

### Tokens identificados

- `NUMERO`: Identifica números inteiros, decimais e em notação científica.
- `ESTERIOTIPO_CLASSE`: event, situation, process,category,mixin,phaseMixin, roleMixin, historicalMixin, kind, collective, quantity, quality, mode, intrisicMode, extrinsicMode, subkind, phase, role, historicalRole, relator.
- `ESTERIOTIPO_RELACAO`: material, derivation, comparative, mediation, characterization, externalDependence, componentOf, memberOf, subCollectionOf, subQualityOf, instantiation, termination, participational, participation, historicalDependence, creation, manifestation, bringsAbout, triggers, composition, aggregation, inherence, value, formal, constitution.
- `PALAVRA_RESERVADA`: genset, disjoint, complete, general, specifies, where, package, import, functional-complexes, intrinsic-modes
- `SIMBOLO`: “{“, “}”, “(“, “)”, “[“, “]”, “..”, “<>--” , “--<>”, “*”, “@”, “:”, "--".
- `NOME_DE_CLASSE`: Iniciando com letra maiúscula, seguida por qualquer
combinação de letras, ou tendo sublinhado como subcadeia própria, sem números.
- `NOME_DE_RELACAO`: Começando com letra minúscula, seguida por qualquer
combinação de letras, ou tendo sublinhado como subcadeia própria, sem números.
- `INSTANCIA`: Iniciando com qualquer letra, podendo ter o sublinhado como subcadeia própria e terminando com algum número inteiro.
- `DADO_NATIVO`: number, string, boolean, date, time, datetime.
“DataType”. 
- `NOVO_TIPO`: Iniciando com letra, sem números, sem sublinhado e terminando com a subcadeia
“DataType”. 
- `META_ATRIBUTO`: ordered, const, derived, subsets, redefines.

### Relatório de análise léxica

O próximo arquivo a ser gerado é o `symbol_report.txt`, nele é contabilizado a ocorrência de alguns tokens de forma distinta no código.

```txt
RELATÓRIO DE ANÁLISE LÉXICA
Contagem da quantidade de ocorrências únicas de cada token:
==========================
Total de palavras reservadas: 6
Total de classes: 30
Total de esteriotipos de classes: 9
Total de relações: 11
Total de esteriotipos de relações: 0
Total de instâncias: 0
Total de meta atributos: 0
==========================
```

## Tratamento de exceções

Um erro léxico é identificado quando um determinado lexema não segue nenhum padrão estabelecido pelas expressões regulares no arquivo `lexer.l`, quando isso acontece, o analisador léxico retorna um token `NAO_IDENTIFICADO` na tabela de simbolos, bem como emite um alerta visual na linha de comando sobre o erro encontrado. Por fim, todas as ocorrências de erro léxico são salvas no arquivo `output/error_report.txt` juntamente com a localização do erro no código. 

```txt
RELATÓRIO DE ERROS LÉXICOS
==========================
Erro Léxico: Símbolo não identificado '?' na linha 79, coluna 20
Erro Léxico: Símbolo não identificado '?' na linha 89, coluna 9
Erro Léxico: Símbolo não identificado '?' na linha 93, coluna 21
==========================
```

## Próximos passos

Após a conclusão da análise léxica, que transforma o código-fonte em uma sequência de tokens, o próximo passo natural é o desenvolvimento do analisador sintático. Esta nova fase será responsável por validar a estrutura gramatical da linguagem TONTO.

## Referências

- Aho, Alfred V. Compiladores: Princípios, Técnicas e Ferramentas. 2ª ed. Pearson Addison-Wesley, 2008. ISBN: 978-85-88639-84-9

- Lenke, M., Tonto: A Textual Syntax for OntoUML – A textual way for conceptual modeling. Available
online at: https://matheuslenke.github.io/tonto-docs/

