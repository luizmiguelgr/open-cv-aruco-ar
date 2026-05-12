#ifndef CONTEUDO_HISTORIA_H
#define CONTEUDO_HISTORIA_H

#include "ConteudoEducacional.h"

class ConteudoHistoria : public ConteudoEducacional {
public:
    ConteudoHistoria(const std::string& titulo,
                     const std::string& descricao,
                     Objeto3D* modelo);

    std::string getCuriosidade() const override;
};

#endif
