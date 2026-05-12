#include "ConteudoGeografia.h"

using namespace std;
using namespace cv;

// materia compartilhada por todos os conteudos de Geografia: azul
static Materia kMateriaGeo("GEOGRAFIA", Scalar(220, 130, 30));

ConteudoGeografia::ConteudoGeografia(const string& titulo,
                                     const string& descricao,
                                     Objeto3D* modelo)
    : ConteudoEducacional(kMateriaGeo, titulo, descricao, modelo) {}

string ConteudoGeografia::getCuriosidade() const {
    return "[GEO] " + descricao;
}
