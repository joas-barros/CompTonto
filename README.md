# CompTonto

Este repositório tem como finalidade hospedar a implementação, juntamente com a documentação, de um compilador para a linguagem TONTO sendo o foco do momento no analisador léxico

## Sumário

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
TONTOPILER
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


## Resultado gerado pelo analisador

## Tratamento de exceções

## Próximos passos

## Referencias
