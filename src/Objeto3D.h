#ifndef OBJETO3D_H
#define OBJETO3D_H

#include <opencv2/opencv.hpp>

// ============================================================
// Classe base abstrata - representa qualquer objeto 3d que a
// gente queira desenhar em cima do marcador ArUco.
// Toda subclasse precisa implementar o metodo desenhar().
// ============================================================
class Objeto3D {
protected:
    float tamanho;      // tamanho do objeto em metros
    cv::Scalar cor;     // cor das linhas (BGR no OpenCV)

public:
    Objeto3D(float tamanho, cv::Scalar cor) {
        this->tamanho = tamanho;
        this->cor = cor;
    }

    // destrutor virtual pq tem heranca (delete polimorfico)
    virtual ~Objeto3D() {}

    // metodo virtual puro - obriga as filhas a implementarem
    virtual void desenhar(cv::Mat& frame,
                          const cv::Mat& cameraMatrix,
                          const cv::Mat& distCoeffs,
                          const cv::Vec3d& rvec,
                          const cv::Vec3d& tvec) = 0;

    // nome do objeto - tambem virtual pra cada filha dizer o seu
    virtual std::string getNome() const = 0;

    // getters basicos
    float getTamanho() const { return tamanho; }
    cv::Scalar getCor() const { return cor; }
};

#endif
