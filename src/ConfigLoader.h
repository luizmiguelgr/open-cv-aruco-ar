#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include "GerenciadorConteudo.h"

// ============================================================
// ConfigLoader - le um arquivo texto e popula o gerenciador.
//
// Formato de cada linha (campos separados por '|'):
//   materia | id | tipo | arquivo | escala | rotX,rotY,rotZ | offX,offY,offZ | R,G,B | titulo | descricao
//
// materia : Biologia | Historia | Geografia
// tipo    : cubo | piramide | obj
// arquivo : caminho do .obj (use '-' para cubo/piramide)
//
// Linhas que comecam com # sao comentario. Linhas vazias sao ignoradas.
// ============================================================
class ConfigLoader {
public:
    // le o arquivo, popula o gerenciador. Retorna quantos conteudos foram carregados.
    static int carregar(const std::string& caminho, GerenciadorConteudo& gerenciador);
};

#endif
