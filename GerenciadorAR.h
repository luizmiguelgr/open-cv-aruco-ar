#pragma once
#include "ObjetoAR.h"
#include "ObjetoHistoria.h"
#include "ObjetoAnatomia.h"
#include <map>

// Gerenciador de ativos AR
// Usa std::map para vincular ID do marcador ao objeto 3D correspondente
class GerenciadorAR {

private:
    // map com ponteiro para classe base (polimorfismo)
    // chave: ID do marcador | valor: ponteiro para ObjetoAR
    map<int, ObjetoAR*> objetos;

public:
    // Construtor: registra todos os objetos e seus IDs
    GerenciadorAR() {
        // ID 0 -> Torre Eiffel (historia)
        objetos[0] = new TorreEiffelAR();
        objetos[1] = new EstatuaAR();
        objetos[2] = new CranioAR();
    }

    // Destrutor: libera a memória de todos os objetos
    ~GerenciadorAR() {
        for (auto& par : objetos) {
            delete par.second;
        }
    }

    // Retorna o objeto correspondente ao ID do marcador
    // Retorna nullptr se o ID não estiver cadastrado
    ObjetoAR* getObjeto(int id) {
        if (objetos.count(id)) {
            return objetos[id];
        }
        return nullptr;
    }
};