#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

// ============================================================
// Classe Camera - encapsula a webcam e os parametros intrinsecos
// (matriz da camera + coeficientes de distorcao).
// Aqui usamos uma aproximacao sem calibracao.
// ============================================================
class Camera {
private:
    cv::VideoCapture cap;
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

public:
    Camera(int idCamera = 0);
    ~Camera();

    bool estaAberta() const;
    bool capturarFrame(cv::Mat& frame);

    const cv::Mat& getCameraMatrix() const { return cameraMatrix; }
    const cv::Mat& getDistCoeffs() const { return distCoeffs; }
};

#endif
