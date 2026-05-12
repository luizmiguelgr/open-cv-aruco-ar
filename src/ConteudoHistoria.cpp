#include "ConteudoHistoria.h"

using namespace std;
using namespace cv;

// materia compartilhada por todos os conteudos de Historia: laranja
static Materia kMateriaHist("HISTORIA", Scalar(0, 140, 230));

ConteudoHistoria::ConteudoHistoria(const string& titulo,
                                   const string& descricao,
                                   Objeto3D* modelo)
    : ConteudoEducacional(kMateriaHist, titulo, descricao, modelo) {}

string ConteudoHistoria::getCuriosidade() const {
    return "[HIST] " + descricao;
}
