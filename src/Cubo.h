#ifndef CUBO_H
#define CUBO_H

#include "Objeto3D.h"

// ============================================================
// Classe Cubo - herda de Objeto3D e desenha um cubo 3d
// projetando os 8 vertices na tela usando projectPoints.
// ============================================================
class Cubo : public Objeto3D {
public:
    Cubo(float tamanho, cv::Scalar cor);

    void desenhar(cv::Mat& frame,
                  const cv::Mat& cameraMatrix,
                  const cv::Mat& distCoeffs,
                  const cv::Vec3d& rvec,
                  const cv::Vec3d& tvec) override;

    std::string getNome() const override;
};

#endif
