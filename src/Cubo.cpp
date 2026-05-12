#include "Cubo.h"

using namespace std;
using namespace cv;

// chama o construtor da classe pai pra inicializar tamanho e cor
Cubo::Cubo(float tamanho, Scalar cor) : Objeto3D(tamanho, cor) {}

string Cubo::getNome() const {
    return "Cubo";
}

void Cubo::desenhar(Mat& frame,
                    const Mat& cameraMatrix,
                    const Mat& distCoeffs,
                    const Vec3d& rvec,
                    const Vec3d& tvec) {

    float s = tamanho;

    // 8 vertices do cubo no espaco 3d
    vector<Point3f> verticesCubo = {
        {-s, -s, 0},   {s, -s, 0},   {s, s, 0},   {-s, s, 0},      // base
        {-s, -s, -s*2},{s, -s, -s*2},{s, s, -s*2},{-s, s, -s*2}    // topo
    };

    // projeta os pontos 3d no plano 2d da camera
    vector<Point2f> pontosImagem;
    projectPoints(verticesCubo, rvec, tvec,
                  cameraMatrix, distCoeffs, pontosImagem);

    // desenha as 4 arestas da base
    for (int i = 0; i < 4; i++) {
        line(frame, pontosImagem[i], pontosImagem[(i+1) % 4], cor, 2);
    }

    // desenha as 4 arestas do topo
    for (int i = 0; i < 4; i++) {
        line(frame, pontosImagem[i+4], pontosImagem[(i+1) % 4 + 4], cor, 2);
    }

    // desenha as 4 arestas laterais ligando base e topo
    for (int i = 0; i < 4; i++) {
        line(frame, pontosImagem[i], pontosImagem[i+4], cor, 2);
    }
}
