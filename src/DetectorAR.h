#ifndef DETECTOR_AR_H
#define DETECTOR_AR_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "Camera.h"
#include "MarcadorAruco.h"

// ============================================================
// Classe DetectorAR - junta tudo: usa a Camera, roda o detector
// do ArUco e devolve uma lista de MarcadorAruco com pose pronta.
// (padrao de projeto: Facade - esconde a complexidade do ArUco
// atras de uma interface simples)
// ============================================================
class DetectorAR {
private:
    Camera& camera;                       // referencia pra camera
    float ladoMarcador;                   // tamanho do marcador em metros
    cv::aruco::Dictionary dicionario;
    cv::aruco::DetectorParameters parametros;
    cv::aruco::ArucoDetector detector;

public:
    DetectorAR(Camera& cam, float ladoMarcador);

    // detecta marcadores no frame e ja estima a pose de cada um
    std::vector<MarcadorAruco> detectar(const cv::Mat& frame);

    // desenha contorno dos marcadores detectados
    void desenharMarcadores(cv::Mat& frame,
                            const std::vector<MarcadorAruco>& marcadores);

    float getLadoMarcador() const { return ladoMarcador; }
};

#endif
