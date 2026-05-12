#ifndef MODELO_OBJ_H
#define MODELO_OBJ_H

#include "Objeto3D.h"
#include "ParserOBJ.h"

// ============================================================
// ModeloOBJ - herda de Objeto3D e desenha em WIREFRAME um modelo
// 3d carregado de um arquivo .obj.
// Parametros de transform local (escala, rotacao, offset) deixam
// cada modelo ser ajustado individualmente ao seu marcador.
// ============================================================
class ModeloOBJ : public Objeto3D {
private:
    MalhaOBJ malha;
    std::string nome;
    float escala;
    cv::Vec3f rotacao;  // graus em X,Y,Z
    cv::Vec3f offset;   // em metros
    bool carregado;

    // monta a matriz 3x3 de rotacao local a partir de rotacao em graus
    cv::Matx33f montarRotLocal() const;

public:
    ModeloOBJ(const std::string& caminho,
              const std::string& nome,
              float escala,
              cv::Vec3f rotacao,
              cv::Vec3f offset,
              cv::Scalar cor);

    void desenhar(cv::Mat& frame,
                  const cv::Mat& cameraMatrix,
                  const cv::Mat& distCoeffs,
                  const cv::Vec3d& rvec,
                  const cv::Vec3d& tvec) override;

    std::string getNome() const override { return nome; }
    bool estaCarregado() const { return carregado; }
};

#endif
