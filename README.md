# CompTonto

Este repositório tem como finalidade hospedar a implementação, juntamente com a documentação, de um compilador para a linguagem TONTO sendo o foco do momento no analisador semantico para validação de padrões de projeto de ontologia.

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
* [Análise semântica](#analise-semântica)
  * [Padrões de projeto reconhecidos](#padrões-de-projeto-reconhecidos)
  * [Erros de modelagem reconhecidos](#erros-de-modelagem-reconhecidos)
  * [Arquivos de saída da análise semântica](#arquivos-de-saída-da-análise-semâtica)
* [Considerações finais](#considerações-finais)
* [Referências](#referências)

## Objetivos

TONTO é o acrônimo para **Textual Ontology Language**, uma linguagem projetada para especificar textualmente ontologias como artefatos computacionais.

Na primeira etapa do projeto, realizamos a implementação de um **analisador léxico** para a linguagem TONTO. Esta ferramenta é a primeira etapa no processo de compilação, sendo responsável por processar o código-fonte e reconhecê-lo como uma sequência de unidades léxicas (ou *tokens*). Nessa parte, criamos um subconjunto da
linguagem Tonto com regras especíﬁcas de escrita de lexemas para representar classes, relações, instâncias, simbolos especiais, além de reconhecer padrões para identificadores de classes, relações, dentre outras coisas.

Na segunda etapa, implementamos o **analisador sintático** da linguagem, cujo principal objetivo é validar se a sequência de tokens gerada na etapa anterior obedece às regras gramaticais da linguagem TONTO. Além da validação, o analisador é responsável por estruturar hierarquicamente os dados (pacotes, classes, relações e generalizações), garantindo a coerência na formação das frases da linguagem. Portanto, na fase de análise sintática, teremos de orientar o ontologista a escrever as estruturas que deﬁnem cada construto da linguagem de acordo com regras bem deﬁnidas de escopo e ordem.

Por fim, temos a implementação do **analisador semântico**, cujo objetivo será validar padrões de projeto de ontologias (ontology design patterns - ODPs) que possam estar presentes no código. Os ODPs conferem uma estrutura lógica mais formal à especiﬁcação da ontologia, pois deﬁnem regras de combinação dos conceitos e relações.

Ao final da implementação, nosso **analisador semântico**, juntamente com os outros analisadores desenvolvidos anteriormente, deve ser capaz de reconhecer e validar os seguintes padrões presentes em uma ontologia monobloco:

- `Subkind pattern`
- `Role pattern`
- `Phase pattern`
- `Relator pattern`
- `Mode pattern`
- `RoleMixin pattern`

## Tecnologias utilizadas

- **Flex**: (Ferramenta de análise léxica)
- **Bison**: (Ferramenta de análise sintática)
- **C++17**: Linguagem de programação e versão utilizada
- **CMake**: Ferrmanta para gerar os arquivos de build (Versão 3.0 ou superior)
- **G++**: Compilador da linguagem C++ para linux
- **GBD**: Ferramenta de depuração
- **Visual Studio Code**: Editor de código 
- **Git**: Versionamento do código

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
├── examples/                # Exemplos de ontologias TONTO
├── include/                 # Cabeçalhos do projeto (arquivos .h)
│   ├── colors.h             # Definições de cores para o terminal
│   ├── file_generator.h     # Interface para geração de relatórios e JSON
│   ├── semantic_analyzer.h  # Interface do analisador semântico (ODPs)
│   ├── symbol_table.h       # Declarações da tabela de símbolos (Léxico)
│   ├── synthesis_table.h    # Declarações da tabela de síntese (Sintático)
│   └── tokens.h             # Definição centralizada dos tokens
├── output/                  # Saídas geradas pelo compilador
│   ├── lexical/             # Relatórios da análise léxica
│   │   ├── symbol_table.json
│   │   └── error_report.txt
│   ├── syntactical/         # Relatórios da análise sintática
│   │   ├── synthesis_table.json
│   │   └── syntactical_structure_report.txt
│   └── semantic/            # Relatórios da análise semântica (ODPs)
│       ├── semantic_analysis_report.txt
│       └── semantic_issues_report.txt
├── src/                     # Implementações principais (arquivos .cpp)
│   ├── file_generator.cpp   # Implementação da geração de arquivos de saída
│   ├── semantic_analyzer.cpp# Implementação do analisador semântico
│   ├── symbol_table.cpp     # Implementação da tabela de símbolos
│   └── synthesis_table.cpp  # Implementação da lógica sintática
├── .gitignore               # Arquivo para ignorar arquivos/pastas no Git
├── CMakeLists.txt           # Configuração do build system CMake
├── lexer.l                  # Especificação Flex (Analisador Léxico)
├── parser.y                 # Especificação Bison (Gramática)
├── parser.tab.c             # Arquivo gerado pelo Bison
├── README.md                # Documentação principal do projeto
└── teste.tonto              # Arquivo de teste da ontologia
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

## Análise Sintática

Após a conclusão da análise léxica, que transforma o código-fonte em uma sequência de tokens, o próximo passo natural é o desenvolvimento do analisador sintático. Esta nova fase será responsável por validar a estrutura gramatical da linguagem TONTO.

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

  * **Relações**: Podem ser declaradas de duas formas distintas:

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

## Analise Semântica

Após a etapa de analise sintática, com a construção da nossa `synthesis_table` com todos nossos construtos reconhecidos, e estruturados conforme o relacionamento de cada um dentro da ontologia, é possível realizar o reconhecimento de padrões de projeto presentes na mesma por meio do nosso `analisador semântico`, cujo processo de análise e reconhecimento pode ser dividido da seguinte forma:

1. Recebe a `synthesis_table` com todos os construtos estruturados
2. Percorre toda a ontologia reconhecendo um padrão por vez
3. Verifica se algo está fora do padrão e armazena o possivel erro para ser exibido ao ontologista depois
4. Após todo processo de analise semântica, são gerados dois relatórios, um com os padrões de projeto reconhecidos corretamente dentro da ontologia e outro com os erros encontrados de acordo com os padrões estabelecidos.

Todo processo de análise semântica é feita dentro da classe `SemanticAnalyzer` declarada em `include\semantic_analiser.h` e implementada em `src\semantic_analiser.cpp`

### Padrões de projeto reconhecidos:

1. **Subkind pattern**: Padrão de projeto que define uma estrutura de hierarquia entre uma classe de esteriotipo `subkind` e outra classe pai, sendo que se essa classe tiver mais de um filho `subkind`, deve ser declarada alguma generalização de relacionamento entre elas dentro da ontologia.
    - **Estrutura TONTO do padrão**:
    ```txt
        package Subkind_Pattern

        kind ClassName
        subkind SubclassName1 specializes ClassName
        subkind SubclassName2 specializes ClassName

        disjoint complete genset Kind_Subkind_Genset_Name { 
            general ClassName
            specifics SubclassName1, SubclassName2
        }
        // "complete" is optional, but "disjoint" applies to subkinds
      ```

2.  **Role pattern**: Padrão que define uma estrutura de hierarquia onde uma classe pai é especializada por duas ou mais classes com estereótipo `role`. Devido à natureza relacional e dinâmica dos papéis, é obrigatório haver um `genset` agrupando essas classes, porém, este genset **não** deve possuir a restrição `disjoint` (podendo ser `overlapping` ou indefinido quanto à disjunção).

      - **Estrutura TONTO do padrão**:

    <!-- end list -->

    ```txt
        package Role_Pattern

        kind ClassName
        role Role_Name1 specializes ClassName
        role Role_Name2 specializes ClassName

        complete genset Class_Role_Genset_Name { 
            general ClassName
            specifics Role_Name1, Role_Name2
        }
        // "complete" is optional, but "disjoint" doesn't apply to roles here
    ```

3.  **Phase pattern**: Padrão que representa mudanças de estados intrínsecos de uma entidade. Define uma hierarquia onde uma classe pai é especializada por duas ou mais classes com estereótipo `phase`. Para este padrão, a declaração de um `genset` é obrigatória e ele **deve** conter a restrição `disjoint` (fases são mutuamente exclusivas).

      - **Estrutura TONTO do padrão**:

    <!-- end list -->

    ```txt
        package Phase_Pattern

        kind ClassName
        phase Phase_Name1 specializes ClassName
        phase Phase_Name2 specializes ClassName

        disjoint complete genset Class_Phase_Genset_Name {
            general ClassName
            specifics Phase_Name1, Phase_Name2
        }
        // "disjoint" is mandatory for phases
    ```

4.  **Relator pattern**: Padrão complexo que envolve uma classe com estereótipo `relator` atuando como mediadora entre outras entidades. O relator deve possuir relações internas de `@mediation` apontando para duas ou mais classes distintas. Adicionalmente, deve existir uma relação externa de `@material` conectando as classes que estão sendo mediadas.

      - **Estrutura TONTO do padrão**:

    <!-- end list -->

    ```txt
        package Relator_Pattern

        kind ClassName1
        kind ClassName2

        relator Relator_Name {
            @mediation [1..*] -- [1..*] ClassName1
            @mediation [1..*] -- [1..*] ClassName2
        }

        @material relation ClassName1 [1..*] -- relationName -- [1..*] ClassName2 
    ```

5.  **Mode pattern**: Padrão que define uma propriedade intrínseca, mas dependente, de uma entidade. Consiste em uma classe com estereótipo `mode` que deve possuir obrigatoriamente uma relação interna de `@characterization` apontando para a classe que ela caracteriza. Pode opcionalmente conter relações de `@externalDependence`.

      - **Estrutura TONTO do padrão**:

    <!-- end list -->

    ```txt
        package Mode_Pattern

        kind ClassName1
        kind ClassName2

        mode Mode_Name1 {
            @characterization [1..*] -- [1] ClassName1
            @externalDependence [1..*] -- [1] ClassName2
        }
    ```

6.  **RoleMixin pattern**: Padrão que define uma abstração de papéis comuns a diferentes tipos. A estrutura exige que uma classe pai tenha o estereótipo `roleMixin` e seja especializada por duas ou mais classes com estereótipo `role`. É obrigatória a presença de um `genset` com a restrição `disjoint` agrupando os filhos.

      - **Estrutura TONTO do padrão**:

    <!-- end list -->

    ```txt
        package RoleMixin_Pattern

        kind ClassName1
        kind ClassName2

        role Role_Name1 specializes ClassName1
        role Role_Name2 specializes ClassName2

        roleMixin RoleMixin_Name

        disjoint complete genset RoleMixin_Genset_Name {
            general RoleMixin_Name
            specifics Role_Name1, Role_Name2
        }
    ```

### Erros de modelagem reconhecidos:

O analisador semântico identifica violações específicas de regras ontológicas e estruturais para cada padrão. Abaixo estão os erros reportados no arquivo `output\semantic\semantic_issues_report.txt`:

1.  **Hierarquia Inválida: Erro de Coerção**

      * **Contexto**: Ocorre no *Subkind Pattern*.
      * **Descrição**: Uma classe é declarada com estereótipo `subkind` mas não especializa nenhuma outra classe. Semanticamente, um subkind é uma especialização rígida e *deve* herdar o princípio de identidade de um `kind` (ou outro tipo rígido).
      * **Exemplo que gera erro**:
        ```txt
        subkind Cachorro // Erro: Não especifica "specializes Animal"
        ```

2.  **Declaração Incompleta**

      * **Contexto**: Todos os padrões (Subkind, Role, Phase, Relator, RoleMixin).
      * **Descrição**: Indica que a estrutura mínima do padrão não foi atendida. As causas mais comuns são:
          * Falta de um `genset` agrupando múltiplas especializações (obrigatório para *Role*, *Phase*, *RoleMixin* e *Subkind* com múltiplos filhos).
          * Quantidade insuficiente de filhos (ex: um *RoleMixin* com apenas 1 filho, ou um *Relator* mediando apenas 1 classe).
      * **Exemplo que gera erro**:
        ```txt
        kind Pessoa
        role Estudante specializes Pessoa
        role Professor specializes Pessoa
        // Erro: Faltou declarar o genset agrupando Estudante e Professor
        ```

3.  **Violação de Anti-Rigidez**

      * **Contexto**: Ocorre no *Role Pattern*.
      * **Descrição**: Um `genset` que agrupa classes `role` foi declarado com a restrição `disjoint`. Como *Roles* são tipos anti-rígidos e relacionais (um mesmo indivíduo pode desempenhar múltiplos papéis simultaneamente, ex: ser Professor e Aluno), a disjunção não deve ser forçada.
      * **Exemplo que gera erro**:
        ```txt
        disjoint complete genset Papeis { // Erro: "disjoint" não se aplica aqui
            general Pessoa
            specifics Estudante, Professor
        }
        ```

4.  **Violação Temporal**

      * **Contexto**: Ocorre no *Phase Pattern*.
      * **Descrição**: Um `genset` que agrupa classes `phase` foi declarado **sem** a restrição `disjoint`. Fases representam estados intrínsecos e mutuamente exclusivos no tempo (ex: Vivo/Morto, Criança/Adulto), portanto a disjunção é obrigatória.
      * **Exemplo que gera erro**:
        ```txt
        complete genset FasesDaVida { // Erro: Faltou "disjoint"
            general Pessoa
            specifics Crianca, Adulto
        }
        ```

5.  **Dependência Existencial**

      * **Contexto**: Ocorre no *Mode Pattern*.
      * **Descrição**: Uma classe `mode` foi declarada sem uma relação interna de `@characterization`. Modes são propriedades intrínsecas que dependem existencialmente de um portador (o "bearer"), logo essa relação é obrigatória.
      * **Exemplo que gera erro**:
        ```txt
        mode Habilidade {
            // Erro: Faltou declarar @characterization ...
        }
        ```

6.  **Tipo Inválido de Especialização: Erro de Coerção**

      * **Contexto**: Ocorre no *RoleMixin Pattern*.
      * **Descrição**: Uma classe `roleMixin` está sendo especializada por uma classe que não possui o estereótipo `role`. Um *RoleMixin* é uma abstração de papéis, portanto, seus filhos concretos devem ser, por definição, papéis.
      * **Exemplo que gera erro**:
        ```txt
        roleMixin Cliente
        kind Pessoa specializes Cliente // Erro: "kind" não pode herdar de RoleMixin
        ```

7.  **Abstração de Papéis**

      * **Contexto**: Ocorre no *RoleMixin Pattern*.
      * **Descrição**: O `genset` que agrupa os filhos de um *RoleMixin* não possui a restrição `disjoint`. Para garantir a correta abstração e separação dos tipos que jogam aquele papel (geralmente tipos disjuntos como *Pessoa* e *Organização*), a disjunção é exigida neste padrão.
      * **Exemplo que gera erro**:
        ```txt
        complete genset Clientes { // Erro: Faltou "disjoint"
            general Cliente
            specifics ClientePessoa, ClienteEmpresa
        }
        ```

### Arquivos de saída da análise semâtica

Ao final do processo de análise, são gerados os seguintes arquivos:

  - **Relatório contendo todos os padrões completos encontrados no código: em `output/semantic/semantic_analysis_report.txt`:**
    ```txt
        RELATÓRIO DE ANÁLISE SEMÂNTICA
        ==========================

        Padrão: Subkind Pattern
        Status: COMPLETE
        Descrição: A classe 'Bebida' possui especializações do tipo 'subkind' e um genset ('disjoint complete') de nome 'Opcao_De_Bebida' que as agrupa.
        Participantes:
          general: Bebida
          specifics: Agua, Refrigerante, Suco
        --------------------------

      ```

  - **Relatório contendo os padrão encontrados no código incompleto ou com algum erro de padrão em `output/semantic/semantic_issues_report.txt`**
  
    ```txt
        RELATÓRIO DE PROBLEMAS SEMÂNTICOS
        ==========================

        Padrão: Role Pattern
        Status: Declaração Incompleta
        Descrição do Problema: A classe 'Empresa' possui 2 roles, mas não há um 'genset' declarado (obrigatório para o padrão Role).
        Participantes:
          general: Empresa
          roles: ClienteEmpresarial, Pizzaria
        --------------------------
      ```


## Considerações finais:

O trabalho cumpriu todos os objetivos descritos no início da documentação, sendo capaz de reconhecer todos os padrões de projeto de ontologia previstos, além de dar uma visão ampla dos erros semânticos presentes no código com base nos padrões definidos. Tudo isso foi alcançado implementando todas as etapas de um front-end de compilador para a linguagem TONTO, integrando as fases de análise léxica, sintática e semântica.

Os principais pontos de melhoria para projetos futuros são: suporte a múltiplos arquivos de entrada para ontologias modulares; melhor tratamento de erros sintáticos (já que o compilador atual para no primeiro erro que encontra); e, por fim, a implementação de um back-end para geração de código em linguagens como OWL ou gUFO.


## Referências

- Aho, Alfred V. Compiladores: Princípios, Técnicas e Ferramentas. 2ª ed. Pearson Addison-Wesley, 2008. ISBN: 978-85-88639-84-9

- Lenke, M., Tonto: A Textual Syntax for OntoUML – A textual way for conceptual modeling. Available
online at: https://matheuslenke.github.io/tonto-docs/

- Ruy, F. B., Guizzardi, G., Falbo, R. A., Reginato, C. C., & Santos, V. A. (2017). From reference
ontologies to ontology patterns and back. Data & Knowledge Engineering, 109, 41-69.

