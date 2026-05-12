#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "GerenciadorAR.h"

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(0); // abre primeira câmera do dispositivo
    if (!cap.isOpened()) {
        cout << "Erro ao abrir câmera!" << endl;
        return -1;
    }

    // Matriz de câmera padrão (aproximação sem calibração)
    // fx, fy = distância focal | cx, cy = centro da imagem
    Mat cameraMatrix = (Mat_<double>(3,3) << 800, 0, 320, 0, 800, 240, 0, 0, 1);

    // Coeficientes de distorção zerados (sem calibração)
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);

    // Tamanho físico do marcador em metros
    float markerLength = 0.05f; // 5 centímetros

    // Conjunto de marcadores ArUco 4x4
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    aruco::DetectorParameters detectorParams;
    aruco::ArucoDetector detector(dictionary, detectorParams);

    // Gerenciador de ativos: vincula IDs aos objetos 3D
    GerenciadorAR gerenciador;

    Mat frame; // matriz de pixels

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        vector<vector<Point2f>> corners; // 4 vértices de cada marcador
        vector<int> ids;                 // IDs dos marcadores detectados
        vector<vector<Point2f>> rejected; // regiões descartadas

        detector.detectMarkers(frame, corners, ids, rejected);

        if (!ids.empty()) {
            aruco::drawDetectedMarkers(frame, corners, ids);

            for (int i = 0; i < (int)ids.size(); i++) {
                Vec3d rvec, tvec;

                // os 4 cantos do marcador no espaço 3D real (em metros)
                vector<Point3f> objPoints = {
                    {-markerLength/2,  markerLength/2, 0},
                    { markerLength/2,  markerLength/2, 0},
                    { markerLength/2, -markerLength/2, 0},
                    {-markerLength/2, -markerLength/2, 0}
                };

                // calcula rotação e posição do marcador no espaço 3D
                solvePnP(objPoints, corners[i], cameraMatrix, distCoeffs, rvec, tvec);

                // desenha eixos XYZ sobre o marcador
                drawFrameAxes(frame, cameraMatrix, distCoeffs, rvec, tvec, markerLength);

                // busca o objeto 3D vinculado ao ID detectado
                ObjetoAR* objeto = gerenciador.getObjeto(ids[i]);

                // se o objeto existir, renderiza ele
                if (objeto != nullptr) {
                    objeto->render(frame, cameraMatrix, distCoeffs, rvec, tvec);
                }

                cout << "ID: " << ids[i] << " | objeto: ";
                cout << (objeto ? objeto->getNome() : "nao cadastrado") << endl;
            }
        }

        imshow("ArUco AR", frame);

        if (waitKey(1) == 27) break; // ESC sai
    }

    cap.release();
    destroyAllWindows();
    return 0;
}