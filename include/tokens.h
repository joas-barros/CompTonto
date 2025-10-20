#ifndef TOKENS_H
#define TOKENS_H

#include <string>

// Constantes para os tipos de tokens
enum Token {
    NUMERO = 1,
    ESTERIOTIPO_CLASSE,
    ESTERIOTIPO_RELACAO,
    PALAVRA_RESERVADA,
    SIMBOLO,
    NOME_DE_CLASSE,
    NOME_DE_RELACAO,
    INSTANCIA,
    DADO_NATIVO,
    NOVO_TIPO,
    META_ATRIBUTO,
    NAO_IDENTIFICADO
};

inline std::string tokenToString(Token token) {
    switch (token) {
        case NUMERO:              return "NUMERO";
        case ESTERIOTIPO_CLASSE:  return "ESTERIOTIPO_CLASSE";
        case ESTERIOTIPO_RELACAO: return "ESTERIOTIPO_RELACAO";
        case PALAVRA_RESERVADA:   return "PALAVRA_RESERVADA";
        case SIMBOLO:             return "SIMBOLO";
        case NOME_DE_CLASSE:      return "NOME_DE_CLASSE";
        case NOME_DE_RELACAO:     return "NOME_DE_RELACAO";
        case INSTANCIA:           return "INSTANCIA";
        case DADO_NATIVO:         return "DADO_NATIVO";
        case NOVO_TIPO:           return "NOVO_TIPO";
        case META_ATRIBUTO:       return "META_ATRIBUTO";
        case NAO_IDENTIFICADO:    return "NAO_IDENTIFICADO";
        default:                  return "TOKEN_DESCONHECIDO"; 
    }
}

#endif 