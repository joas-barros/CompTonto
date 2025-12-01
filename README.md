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
* [Análise léxica](#análise-léxica)
  * [Tokens Identificados](#tokens-identificados)
  * [Tabela de símbolos](#tabela-de-símbolos)
  * [Relatório de análise léxica](#relatório-de-análise-léxica)
  * [Relatório de erros léxicos](#relatório-de-erros-léxicos)
* [Análise sintática](#análise-sintática)
  * [Principais Construtos Reconhecidos](#principais-construtos-reconhecidos)
  * [Saída do analisador](#saída-do-analisador)
  * [Tratamento de erros](#tratamento-de-erros)
* [Principais limitações do analisador](#principais-limitações-de-analisador)
* [Referências](#referências)

## Objetivos

TONTO é o acrônimo para **Textual Ontology Language**, uma linguagem projetada para especificar textualmente ontologias como artefatos computacionais.

Na primeira etapa do projeto, realizamos a implementação de um **analisador léxico** para a linguagem TONTO. Esta ferramenta é a primeira etapa no processo de compilação, sendo responsável por processar o código-fonte e reconhecê-lo como uma sequência de unidades léxicas (ou *tokens*). Nessa parte, criamos um subconjunto da
linguagem Tonto com regras especíﬁcas de escrita de lexemas para representar classes, relações, instâncias, simbolos especiais, além de reconhecer padrões para identificadores de classes, relações, dentre outras coisas.

Na atual etapa do projeto, a segunda etapa, cujo principal objetivo é validar se a sequência de tokens gerada na etapa anterior obedece às regras gramaticais da linguagem TONTO. Além da validação, o analisador é responsável por estruturar hierarquicamente os dados (pacotes, classes, relações e generalizações), garantindo a coerência na formação das frases da linguagem. Portanto, fase de análise sintática, teremos de orientar um ontologista a escrever as estruturas que deﬁnem cada construto da linguagem de acordo com regras bem deﬁnidas de escopo e ordem.

Dessa forma, cabe a nosso **analisador sintático** da linguagem TONTO verificar a corretude de especificação textual de uma ontologia nos seguintes casos:

* `Declaração de pacotes`
* `Declaração de classes`
* `Declaração de tipos de dados`
* `Declaração de classes enumeradas`
* `Generalizações (Generalization sets)`
* `Declarações de relações`

A implementação deste analisador cumpre dois propósitos principais:

1.  **Geração da tabela de sínteses**: Após a estrutura do código ser validada pela gramática, todos os construtos são salvos na Tabela de Síntese (Synthesis Table) e posteriormente exportados em um arquivo JSON. Este arquivo revela toda a árvore hierárquica da ontologia, organizando pacotes, classes e seus respectivos membros.
2.  **Relatório de erros da ontologia**: O relatório abrange:
    - **Erros Sintáticos**: Identificação de tokens inesperados ou violações da gramática, com indicação exata de linha e coluna, além de listar quais tokens eram esperados naquele contexto.
    - **Erros Estruturais/Semânticos**: Validação da topologia da ontologia, identificando inconsistências lógicas como generalizações apontando para classes inexistentes, relações com origens/destinos inválidos ou duplicidade de declarações. Cada erro acompanha uma mensagem descritiva e uma sugestão de correção.

## Tecnologias utilizadas

- **Flex**: (Ferramenta de análise léxica)
- **Bison**: (Ferramenta de análise sintática)
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
 Build/comptonto teste.tonto
```

O arquivo `teste.tonto` é o arquivo da analise léxica, caso queira mudar o exemplo, basta copiar o conteúdo para dentro de `teste.tonto` e seguir os passos anteriores.

## Estrutura do projeto

```bash
COMPTONTO
├── .vscode/                 # Configurações do Visual Studio Code
├── Build/                   # Diretório de build (arquivos compilados)
├── include/                 # Cabeçalhos do projeto (arquivos .h)
│   ├── colors.h             # Definições de cores para o terminal
│   ├── file_generator.h     # Interface para geração de relatórios e JSON
│   ├── symbol_table.h       # Declarações da tabela de símbolos (Léxico)
│   └── synthesis_table.h    # Declarações da tabela de síntese e estruturas semânticas
├── output/                  # Saídas geradas pelo compilador
│   ├── lexical/             # Relatórios da análise léxica (tokens, erros léxicos)
│   └── syntatical/          # Relatórios da análise sintática (JSON estrutural, erros estruturais)
├── src/                     # Implementações principais (arquivos .cpp)
│   ├── file_generator.cpp   # Implementação da geração de arquivos de saída
│   ├── symbol_table.cpp     # Implementação da tabela de símbolos
│   └── synthesis_table.cpp  # Implementação da lógica de síntese e validação semântica
├── .gitignore               # Arquivo para ignorar arquivos/pastas no Git
├── CMakeLists.txt           # Configuração do build system CMake
├── lexer.l                  # Especificação Flex (Analisador Léxico)
├── parser.y                 # Especificação Bison (Analisador Sintático e Gramática)
├── README.md                # Documentação principal do projeto
└── teste.tonto              # Arquivo de teste de entrada
```


## Análise léxica:

A etapa de análise léxica, implementada utilizando a ferramenta **Flex** (arquivo `lexer.l`), é responsável por escanear o código-fonte e convertê-lo em uma sequência de tokens. As principais saídas e definições desta etapa são:

### Tokens Identificados

Abaixo, a lista atualizada de unidades léxicas reconhecidas pelo compilador, com suas respectivas regras de formação e lexemas:

- **Palavras-Chave de Estrutura:**
  - `IMPORT`: import
  - `PACKAGE`: package
  - `GENSET`: genset
  - `DISJOINT`: disjoint
  - `COMPLETE`: complete
  - `OVERLAPPING`: overlapping
  - `INCOMPLETE`: incomplete
  - `GENERAL`: general
  - `SPECIFICS`: specifics
  - `SPECIALIZES`: specializes
  - `WHERE`: where
  - `ENUM`: enum
  - `RELATION`: relation
  - `DATATYPE`: datatype

- **Identificadores Semânticos:**
  - `ESTERIOTIPO_CLASSE`: event, situation, process, category, mixin, phaseMixin, roleMixin, historicalMixin, kind, collective, quantity, quality, mode, intrisicMode, extrinsicMode, subkind, phase, role, historicalRole, relator, type.
  - `ESTERIOTIPO_RELACAO`: material, derivation, comparative, mediation, characterization, externalDependence, componentOf, memberOf, subCollectionOf, subQualityOf, instantiation, termination, participational, participation, historicalDependence, creation, manifestation, bringsAbout, triggers, composition, aggregation, inherence, value, formal, constitution, subCategoryOf.
  - `DADO_NATIVO`: number, string, boolean, date, time, datetime.
  - `META_ATRIBUTO`: ordered, const, derived, subsets, redefines.
  - `PALAVRA_RESERVADA`: functional-complexes, intrinsic-modes.

- **Identificadores de Usuário:**
  - `NOME_DE_CLASSE`: Inicia com letra maiúscula `[A-Z]`, seguida por letras (maiúsculas ou minúsculas) ou sublinhado `_`. Não contém números.
  - `NOME_DE_RELACAO`: Inicia com letra minúscula `[a-z]`, seguida por letras ou sublinhado `_`. Não contém números.
  - `INSTANCIA`: Inicia com qualquer letra, pode conter sublinhado e **deve** terminar com um ou mais dígitos.
  - `NOVO_TIPO`: Inicia com letra, sem números ou sublinhado, e termina obrigatoriamente com o sufixo "DataType".

- **Literais:**
  - `NUMERO`: Identifica números inteiros, decimais (ponto flutuante) e em notação científica (ex: 10, 3.14, 1.2E+10).

- **Símbolos e Pontuação:**
  - **Delimitadores:** “{“, “}”, “(“, “)”, “[“, “]”.
  - **Setas de Relação:**
    - `LEFT_AGGREGATION_ARROW`: “<>--”
    - `RIGHT_AGGREGATION_ARROW`: “--<>”
    - `LEFT_COMPOSITION_ARROW`: “<o>--”
    - `RIGHT_COMPOSITION_ARROW`: “--<o>”
    - `GENERAL_RELATION_ARROW`: “--”
  - **Outros:**
    - `DOUBLEDOT`: “..”
    - `ASTERISK`: “*”
    - `AT`: “@”
    - `COLON`: “:”
    - `COMMA`: “,”

### Tabela de símbolos

O primeiro arquivo a ser gerado é o `symbol_table.json`, localizado na pasta `output/lexical` nele, estão contidos todos os lexemas encontrados pelo analisador, bem como a qual token ele pertence, e uma lista com a localização de todas as suas ocorrências no código.

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

### Relatório de erros léxicos

Um erro léxico é identificado quando um determinado lexema não segue nenhum padrão estabelecido pelas expressões regulares no arquivo `lexer.l`, quando isso acontece, o analisador léxico retorna um token `NAO_IDENTIFICADO` na tabela de simbolos, bem como emite um alerta visual na linha de comando sobre o erro encontrado. Por fim, todas as ocorrências de erro léxico são salvas no arquivo `output/lexical/error_report.txt` juntamente com a localização do erro no código. 

```txt
RELATÓRIO DE ERROS LÉXICOS
==========================
Erro Léxico: Símbolo não identificado '?' na linha 79, coluna 20
Erro Léxico: Símbolo não identificado '?' na linha 89, coluna 9
Erro Léxico: Símbolo não identificado '?' na linha 93, coluna 21
==========================
```

## Análise sintática

Após a conclusão da análise léxica, que transforma o código-fonte em uma sequência de tokens, o próximo passo natural é o desenvolvimento do analisador sintático. Esta nova fase será responsável por validar a estrutura gramatical da linguagem TONTO.

Com certeza. [cite_start]Utilizando os exemplos e definições presentes no documento de especificação do projeto[cite: 14], aqui está a seção formatada no estilo que você solicitou, focando na estrutura prática e exemplos de uso ao invés de regras gramaticais abstratas.

-----

### Principais Construtos Reconhecidos

O analisador processa e estrutura os seguintes elementos da linguagem TONTO:

  * **Pacotes**: Definem o escopo global ou "visão" da ontologia, funcionando como *namespaces* para os demais elementos.

      * **Estrutura**:
        ```txt
        package MyModel
        ```

  * **Classes**: São o núcleo da ontologia, declaradas com um estereótipo (e.g., kind, role, phase). Suportam declaração de atributos tipados e herança.

      * **Estrutura**:
        ```txt
        kind Person {
            name: string
            birthDate: date {const}
        }

        # Declaração inline com herança:
        phase Child specializes Person
        ```

  * **Tipos de Dados (Data Types)**: Permitem a construção de tipos estruturados complexos a partir dos tipos nativos da linguagem[cite: 28, 29].

      * **Estrutura**:
        ```txt
        datatype Address {
            street: string
            number: int
        }
        ```

  * **Enumerações (Enums)**: Classes com um número finito e pré-definido de instâncias (literais)[cite: 34].

      * **Estrutura**:
        ```txt
        enum EyeColor { Blue, Green, Brown, Black }
        ```

  * **Generalizações (Gensets)**: Definem taxonomias e árvores de herança, podendo conter restrições como `disjoint` e `complete`[cite: 40, 47] O analisador reconhece dois formatos[cite: 43]:

      * **Estrutura (Inline)**:
        ```txt
        disjoint complete genset PersonAgeGroup where Child, Adult specializes Person
        ```
      * **Estrutura (Bloco)**:
        ```txt
        genset PersonAgeGroup {
            general Person
            specifics Child, Adult
        }
        ```

  * **Relações**: Podem ser declaradas de duas formas distintas[cite: 56]:

      * **Interna**: Declarada dentro do escopo de uma classe, assumindo-a como origem[cite: 56].
        ```txt
        kind University {
            @componentOf [1] <>-- [1..*] Department
        }
        ```
      * **Externa**: Declarada no nível do pacote, definindo explicitamente a origem e o destino[cite: 60, 61].
        ```txt
        @mediation relation EmploymentContract [1..*] [1] Employee
        ```
  
### Saída do analisador

Após o processamento do código-fonte, o analisador sintático gera três saídas principais na pasta `output/syntatical`:

1. **Tabela de síntese (JSON)**

O arquivo `synthesis_table.json` contém a árvore hierárquica completa da ontologia. Diferente da tabela de símbolos (léxica), esta estrutura organiza os dados logicamente, agrupando classes dentro de pacotes, atributos dentro de classes e mapeando heranças.

```json
{
  "ontology": {
    "imports": [],
    "packages": [
      {
        "name": "Universidade",
        "classes": [ ... ],
        "data_types": [ ... ],
        "enumerations": [ ... ],
        "generalizations": [ ... ],
        "external_relations": [ ... ]
      }
    ]
  },
  "errors": []
}
```

2. **Reseumo dos construtos (TXT)**:

O arquivo `synthesis_report.txt` contém a quantidade dos principal construtos encontrados na ontologia.

```txt
RELATÓRIO DE ANÁLISE SINTÁTICA
Resumos dos construtos da ontologia:
==========================
Total de imports: 0
Total de pacotes: 1
Total de classes: 8
Total de data types: 0
Total de enumerações: 0
Total de generalizações: 1
Total de relações externas: 0
Total de relações internas: 0
==========================
```

### Tratamento de erros

Existem dois tipos de erros que são tratados no nosso analisador léxico

1. **Erro sintatico**: Caso o código viole a gramática, o compilador informa no terminal:
    - A localização exata (linha e coluna).
    - O token inesperado (com seu lexema).
    - A lista de tokens que eram esperados naquele contexto.
    - **Exemplo**: `Erro de Sintaxe na linha 12: Token inesperado: NUMERO ("10"). Possíveis tokens: COMMA, RIGHT_CURLY_BRACKETS.`

2. **Relatório de Erros Estruturais**: Após a validação gramatical, o compilador executa uma Validação Semântica/Estrutural. O arquivo `syyntactical_structure_report.txt` lista inconsistências lógicas na topologia da ontologia, tais como:
    - Herança de classes não declaradas.
    - Relações apontando para origens ou destinos inexistentes.
    - Generalizações referenciando classes inválidas.

Cada erro é acompanhado de uma mensagem descritiva e uma sugestão de correção para o ontologista.

```txt
RELATÓRIO DE ERROS DE ESTRUTURA
==========================
Erro: Genset 'PhasesOfPerson' refere-se a uma classe filha inexistente: 'Child'. Na linha 0, coluna 0
Sugestão: Corrija o nome da classe specific.
--------------------------
==========================
```
## Principais limitações de analisador:

- Sua gramática só corresponde a uma porção da linguagem TONTO correspondente aos requisitos do projeto.
- Ele só ler um arquivo, então não da pra fazer a validação de multiplas visões com ele.
- Assim que o bison verifica um erro, ele para, não pegando todos os erros sintaticos presentes no arquivo.


## Referências

- Aho, Alfred V. Compiladores: Princípios, Técnicas e Ferramentas. 2ª ed. Pearson Addison-Wesley, 2008. ISBN: 978-85-88639-84-9

- Lenke, M., Tonto: A Textual Syntax for OntoUML – A textual way for conceptual modeling. Available
online at: https://matheuslenke.github.io/tonto-docs/

