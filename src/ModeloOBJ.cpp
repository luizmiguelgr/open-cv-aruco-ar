#include "ModeloOBJ.h"
#include <cmath>
#include <set>

using namespace std;
using namespace cv;

ModeloOBJ::ModeloOBJ(const string& caminho, const string& nome,
                     float escala, Vec3f rotacao, Vec3f offset, Scalar cor)
    : Objeto3D(escala, cor),
      nome(nome),
      escala(escala),
      rotacao(rotacao),
      offset(offset),
      carregado(false)
{
    carregado = ParserOBJ::carregar(caminho, malha);
}

// gera matriz de rotacao Rx*Ry*Rz a partir dos angulos em graus.
Matx33f ModeloOBJ::montarRotLocal() const {
    float dx = rotacao[0] * (float)CV_PI / 180.0f;
    float dy = rotacao[1] * (float)CV_PI / 180.0f;
    float dz = rotacao[2] * (float)CV_PI / 180.0f;

    Matx33f Rx(1, 0, 0,
               0, cos(dx), -sin(dx),
               0, sin(dx),  cos(dx));
    Matx33f Ry( cos(dy), 0, sin(dy),
                0, 1, 0,
               -sin(dy), 0, cos(dy));
    Matx33f Rz(cos(dz), -sin(dz), 0,
               sin(dz),  cos(dz), 0,
               0, 0, 1);

    return Rx * Ry * Rz;
}

void ModeloOBJ::desenhar(Mat& frame,
                         const Mat& cameraMatrix,
                         const Mat& distCoeffs,
                         const Vec3d& rvec,
                         const Vec3d& tvec) {
    if (!carregado || malha.vertices.empty()) return;

    // aplica transform local em cada vertice: escala -> rotaciona -> translada
    Matx33f Rloc = montarRotLocal();
    vector<Point3f> pontosTransformados;
    pontosTransformados.reserve(malha.vertices.size());

    for (const auto& v : malha.vertices) {
        Vec3f p(v.x * escala, v.y * escala, v.z * escala);
        Vec3f r = Rloc * p;
        r[0] += offset[0];
        r[1] += offset[1];
        r[2] += offset[2];
        pontosTransformados.push_back(Point3f(r[0], r[1], r[2]));
    }

    // projeta todos os vertices para 2d de uma vez (rapido)
    vector<Point2f> pontos2D;
    projectPoints(pontosTransformados, rvec, tvec,
                  cameraMatrix, distCoeffs, pontos2D);

    // monta o conjunto de arestas unicas (sem desenhar a mesma 2x)
    // chave: par ordenado (menor, maior) de indices.
    set<pair<int,int>> arestas;
    for (const auto& tri : malha.triangulos) {
        int a = tri[0], b = tri[1], c = tri[2];
        arestas.insert({min(a,b), max(a,b)});
        arestas.insert({min(b,c), max(b,c)});
        arestas.insert({min(a,c), max(a,c)});
    }

    // desenha cada aresta como uma linha 2d
    for (const auto& ar : arestas) {
        line(frame, pontos2D[ar.first], pontos2D[ar.second], cor, 1, LINE_AA);
    }
}
