#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "GerenciadorAR.h"

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Erro ao abrir câmera!" << endl;
        return -1;
    }

    // matriz de câmera aproximada, sem calibração real
    // fx=fy=800 e centro em (320, 240) funciona razoável pra testes
    Mat cameraMatrix = (Mat_<double>(3,3) << 800, 0, 320, 0, 800, 240, 0, 0, 1);
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);

    float markerLength = 0.08f; // 8 centímetros

    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    aruco::DetectorParameters detectorParams;
    aruco::ArucoDetector detector(dictionary, detectorParams);

    GerenciadorAR gerenciador;

    Mat frame;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        vector<vector<Point2f>> corners;
        vector<int> ids;
        vector<vector<Point2f>> rejected;

        detector.detectMarkers(frame, corners, ids, rejected);

        if (!ids.empty()) {
            aruco::drawDetectedMarkers(frame, corners, ids);

            for (int i = 0; i < (int)ids.size(); i++) {
                Vec3d rvec, tvec;

                // cantos do marcador no espaço 3D (sentido horário a partir do canto superior esquerdo)
                vector<Point3f> objPoints = {
                    {-markerLength/2,  markerLength/2, 0},
                    { markerLength/2,  markerLength/2, 0},
                    { markerLength/2, -markerLength/2, 0},
                    {-markerLength/2, -markerLength/2, 0}
                };

                // estima posição e rotação do marcador no espaço 3D
                solvePnP(objPoints, corners[i], cameraMatrix, distCoeffs, rvec, tvec);

                drawFrameAxes(frame, cameraMatrix, distCoeffs, rvec, tvec, markerLength);

                ObjetoAR* objeto = gerenciador.getObjeto(ids[i]);
                if (objeto != nullptr) {
                    objeto->render(frame, cameraMatrix, distCoeffs, rvec, tvec);
                }

            }
        }

        imshow("ArUco AR", frame);

        if (waitKey(1) == 27) break; // ESC pra sair
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
