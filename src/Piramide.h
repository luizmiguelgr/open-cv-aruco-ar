#ifndef PIRAMIDE_H
#define PIRAMIDE_H

#include "Objeto3D.h"

// ============================================================
// Classe Piramide - herda de Objeto3D e desenha uma piramide
// de base quadrada com 4 arestas subindo ate um apice central.
// ============================================================
class Piramide : public Objeto3D {
public:
    Piramide(float tamanho, cv::Scalar cor);

    void desenhar(cv::Mat& frame,
                  const cv::Mat& cameraMatrix,
                  const cv::Mat& distCoeffs,
                  const cv::Vec3d& rvec,
                  const cv::Vec3d& tvec) override;

    std::string getNome() const override;
};

#endif
