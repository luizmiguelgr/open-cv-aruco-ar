#include "ConteudoBiologia.h"

using namespace std;
using namespace cv;

// materia compartilhada por todos os conteudos de Biologia: verde
static Materia kMateriaBio("BIOLOGIA", Scalar(60, 180, 75));

ConteudoBiologia::ConteudoBiologia(const string& titulo,
                                   const string& descricao,
                                   Objeto3D* modelo)
    : ConteudoEducacional(kMateriaBio, titulo, descricao, modelo) {}

string ConteudoBiologia::getCuriosidade() const {
    return "[BIO] " + descricao;
}
