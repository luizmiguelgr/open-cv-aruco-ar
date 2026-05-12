#ifndef MARCADOR_ARUCO_H
#define MARCADOR_ARUCO_H

#include <vector>
#include <opencv2/opencv.hpp>

// ============================================================
// Classe MarcadorAruco - representa um marcador ja detectado,
// guardando id, cantos na imagem e a pose estimada (rvec/tvec).
// ============================================================
class MarcadorAruco {
private:
    int id;
    std::vector<cv::Point2f> cantos;
    cv::Vec3d rvec;   // rotacao
    cv::Vec3d tvec;   // translacao

public:
    MarcadorAruco(int id, const std::vector<cv::Point2f>& cantos);

    // estima a pose do marcador usando solvePnP
    void estimarPose(float ladoMarcador,
                     const cv::Mat& cameraMatrix,
                     const cv::Mat& distCoeffs);

    int getId() const { return id; }
    const cv::Vec3d& getRvec() const { return rvec; }
    const cv::Vec3d& getTvec() const { return tvec; }
    const std::vector<cv::Point2f>& getCantos() const { return cantos; }
};

#endif
