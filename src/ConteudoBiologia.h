#ifndef CONTEUDO_BIOLOGIA_H
#define CONTEUDO_BIOLOGIA_H

#include "ConteudoEducacional.h"

// ============================================================
// ConteudoBiologia - conteudo da materia BIOLOGIA.
// Personaliza a curiosidade com prefixo "[BIO]".
// ============================================================
class ConteudoBiologia : public ConteudoEducacional {
public:
    ConteudoBiologia(const std::string& titulo,
                     const std::string& descricao,
                     Objeto3D* modelo);

    std::string getCuriosidade() const override;
};

#endif
