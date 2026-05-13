#pragma once
#include "ObjetoAR.h"
#include "ObjetoHistoria.h"
#include "ObjetoAnatomia.h"
#include <map>

// mapeia o ID do marcador para o objeto 3D correspondente
class GerenciadorAR {

private:
    map<int, ObjetoAR*> objetos; // ponteiro pra base pra funcionar o polimorfismo

public:
    GerenciadorAR() {
        objetos[0] = new TorreEiffelAR();
        objetos[1] = new EstatuaAR();
        objetos[2] = new CranioAR();
    }

    ~GerenciadorAR() {
        for (auto& par : objetos) {
            delete par.second;
        }
    }

    // retorna nullptr se o ID não tiver cadastrado
    ObjetoAR* getObjeto(int id) {
        if (objetos.count(id)) {
            return objetos[id];
        }
        return nullptr;
    }
};
