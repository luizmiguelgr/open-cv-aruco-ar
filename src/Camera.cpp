#include "Camera.h"

using namespace cv;

Camera::Camera(int idCamera) {
    cap.open(idCamera);

    // matriz padrao - fx,fy = distancia focal, cx,cy = centro da imagem
    cameraMatrix = (Mat_<double>(3,3) <<
        800,   0, 320,
          0, 800, 240,
          0,   0,   1);

    // sem calibracao = distorcao zerada
    distCoeffs = Mat::zeros(5, 1, CV_64F);
}

Camera::~Camera() {
    cap.release();
}

bool Camera::estaAberta() const {
    return cap.isOpened();
}

bool Camera::capturarFrame(Mat& frame) {
    cap >> frame;
    return !frame.empty();
}
