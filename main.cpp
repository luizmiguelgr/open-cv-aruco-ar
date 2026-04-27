#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

void drawCube(Mat& frame, const Mat& cameraMatrix, const Mat& distCoeffs,
                const Vec3d& rvec, const Vec3d& tvec, float size) {
    
    float s = size; //8 vértices do cubo no 3d
    vector<Point3f> cubePoints = {
        {-s,-s, 0}, {s, -s, 0}, {s, s, 0}, {-s, s, 0}, //base
        {-s, -s, -s*2}, {s, -s, -s*2}, {s, s, -s*2},  {-s, s, -s*2} //topo
    };         

    //projeta 3d no plano 2d da câmera
    vector<Point2f> imagePoints;
    projectPoints(cubePoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

    // desenhar arestas da base
    for (int i = 0; i < 4; i++) {
        line(frame, imagePoints[i], imagePoints[(i+1)%4], Scalar(0, 255, 0), 2);
    }

    // desenhar arestas do topo
    for (int i = 0; i < 4; i++) {
        line(frame, imagePoints[i+4], imagePoints[(i+1)%4+4], Scalar(0, 255, 0), 2);
    }

    // desenhar arestas laterais
    for (int i = 0; i < 4; i++) {
        line(frame, imagePoints[i], imagePoints[(i+4)], Scalar(0, 255, 0), 2);
    }
}

int main() {
    VideoCapture cap(0); // abre primeira câmera do dispositivo
    if (!cap.isOpened()) {
        cout << "Erro ao abrir câmera!!!!!!" << endl;
        return -1; // se não abrir câmera, snaliza erro
    }

    //Matriz de câmera padrão (aproximação sem calibração)
    // fx, fy = distância focal | cx, cy = centro da imagem
    Mat cameraMatrix = (Mat_<double>(3,3) <<
        800,   0, 320,
          0, 800, 240,
          0,   0,   1);

    //coeficientes de distorção zerados (sem calibração)
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);

    //tamanho físico do marcador em metros (ajustar)
    float markerLength = 0.05f; // 0.005 = 5 centímetros
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

            // para cada marcador detectado, estima a pose e desenha o cubo
            for (int i = 0; i < (int)ids.size(); i++) {
                Vec3d rvec, tvec;

                //Estima posição (tvec) e rotação (rvec) do marcador no espaço 3d
                // os 4 cantos do marcador no espaço 3D real (em metros)
                // a origem é o centro do marcador
                vector<Point3f> objPoints = {
                    {-markerLength/2,  markerLength/2, 0}, // canto superior esquerdo
                    { markerLength/2,  markerLength/2, 0}, // canto superior direito
                    { markerLength/2, -markerLength/2, 0}, // canto inferior direito
                    {-markerLength/2, -markerLength/2, 0}  // canto inferior esquerdo
                };

                // calcula rvec (rotação) e tvec (posição) comparando os cantos
                // 3D reais com os cantos 2D detectados na câmera
                solvePnP(objPoints, corners[i], cameraMatrix, distCoeffs, rvec, tvec);
                
                //desenha eixos XYZ
                drawFrameAxes(frame, cameraMatrix, distCoeffs, rvec, tvec, markerLength);

                //desenha cubo 3d
                drawCube(frame, cameraMatrix, distCoeffs, rvec, tvec, markerLength / 2);

                cout << "ID: " << ids[i] << " /// tvec: " << tvec << endl;
            }
        }

        imshow("ArUco Detector", frame); //abre a janela e exibe o frame

        if (waitKey(1) == 27) break; // ESC sai do loop
    }

    cap.release(); //libera a câmera

    destroyAllWindows(); //fecha as janelas abertas pelo Opencv

    return 0;
}