#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(0); // abre primeira câmera do dispositivo
    if (!cap.isOpened()) {
        cout << "Erro ao abrir câmera!!!!!!" << endl;
        return -1; // se não abrir câmera, snaliza erro
    }

    //conjunto de marcadores
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    //detecção de parâmetros base (sensibilidade, distorções, etc.)
    aruco::DetectorParameters detectorParams;
    //procurador dos marcadores nos frames
    aruco::ArucoDetector detector(dictionary, detectorParams);

    Mat frame; //matriz de pixels

    while (true) {
        cap >> frame;

        if (frame.empty()) break;

        vector<vector<Point2f>> corners; //lista das coordenadas | 4 vértices
        vector<int> ids; //lista dos IDs numéricos de cada marcador
        vector<vector<Point2f>> rejected; //regiões que pareciam marcadores

        detector.detectMarkers(frame, corners, ids, rejected);

        if (!ids.empty()) {
            aruco::drawDetectedMarkers(frame, corners, ids);
            cout << "Marcador detectado! ID: " << ids[0] << endl;
        }

        imshow("ArUco Detector", frame); //abre a janela e exibe o frame

        if (waitKey(1) == 27) break; // ESC sai do loop
    }

    cap.release(); //libera a câmera

    destroyAllWindows(); //fecha as janelas abertas pelo Opencv

    return 0;
}