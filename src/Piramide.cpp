#include "Piramide.h"

using namespace std;
using namespace cv;

Piramide::Piramide(float tamanho, Scalar cor) : Objeto3D(tamanho, cor) {}

string Piramide::getNome() const {
    return "Piramide";
}

void Piramide::desenhar(Mat& frame,
                        const Mat& cameraMatrix,
                        const Mat& distCoeffs,
                        const Vec3d& rvec,
                        const Vec3d& tvec) {

    float s = tamanho;

    // 5 vertices da piramide: 4 na base + 1 no apice
    vector<Point3f> verticesPiramide = {
        {-s, -s, 0},  // 0 - base canto inf esq
        { s, -s, 0},  // 1 - base canto inf dir
        { s,  s, 0},  // 2 - base canto sup dir
        {-s,  s, 0},  // 3 - base canto sup esq
        { 0,  0, -s*2} // 4 - apice (subindo no eixo Z)
    };

    vector<Point2f> pontosImagem;
    projectPoints(verticesPiramide, rvec, tvec,
                  cameraMatrix, distCoeffs, pontosImagem);

    // desenha as 4 arestas da base
    for (int i = 0; i < 4; i++) {
        line(frame, pontosImagem[i], pontosImagem[(i+1) % 4], cor, 2);
    }

    // desenha as 4 arestas que vao da base ate o apice (indice 4)
    for (int i = 0; i < 4; i++) {
        line(frame, pontosImagem[i], pontosImagem[4], cor, 2);
    }
}
