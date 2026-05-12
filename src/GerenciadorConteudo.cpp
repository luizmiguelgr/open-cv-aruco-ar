#include "GerenciadorConteudo.h"

using namespace std;
using namespace cv;

GerenciadorConteudo::~GerenciadorConteudo() {
    for (auto& par : conteudos) {
        delete par.second;
    }
    conteudos.clear();
}

void GerenciadorConteudo::registrar(int id, ConteudoEducacional* c) {
    auto it = conteudos.find(id);
    if (it != conteudos.end()) {
        delete it->second; // libera o antigo antes de substituir
    }
    conteudos[id] = c;
}

ConteudoEducacional* GerenciadorConteudo::buscar(int id) const {
    auto it = conteudos.find(id);
    return (it == conteudos.end()) ? nullptr : it->second;
}

void GerenciadorConteudo::renderizarPara(int id, Mat& frame,
                                         const Mat& cameraMatrix,
                                         const Mat& distCoeffs,
                                         const Vec3d& rvec,
                                         const Vec3d& tvec) const {
    ConteudoEducacional* c = buscar(id);
    if (c) {
        c->renderizar(frame, cameraMatrix, distCoeffs, rvec, tvec);
    }
}
