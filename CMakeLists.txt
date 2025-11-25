cmake_minimum_required(VERSION 3.10.0)
project(comptonto)

# --- Configuração do C++ ---
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Encontrar Arquivos-Fonte ---
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

# --- Configuração do Flex e Bison ---
# 1. Encontra o pacote do Bison 
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# 2. Define o alvo do Bison
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-d")

# Target do Flex
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS "-+")

# 3. Define a dependência (ADICIONADO)
# Isso garante que o parser.hpp seja gerado ANTES do Flex tentar compilar
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)


# --- Criação do Executável ---
add_executable(comptonto 
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS} 
    ${PROJECT_SOURCES}
)

# --- Diretórios de Inclusão (Headers) ---
# Adiciona os diretórios onde o compilador deve procurar por arquivos .h.
# É a forma moderna e mais segura de fazer isso, aplicando apenas ao target 'comptonto'.
target_include_directories(comptonto PRIVATE
    # Necessário para encontrar o header gerado pelo Flex (ex: lex.yy.h)
    ${CMAKE_CURRENT_BINARY_DIR}

    # Necessário para encontrar seus próprios headers em include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)