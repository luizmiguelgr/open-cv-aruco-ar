#ifndef CONTEUDO_GEOGRAFIA_H
#define CONTEUDO_GEOGRAFIA_H

#include "ConteudoEducacional.h"

class ConteudoGeografia : public ConteudoEducacional {
public:
    ConteudoGeografia(const std::string& titulo,
                      const std::string& descricao,
                      Objeto3D* modelo);

    std::string getCuriosidade() const override;
};

#endif
