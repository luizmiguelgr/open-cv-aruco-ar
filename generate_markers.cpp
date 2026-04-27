#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

int main() {
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);

    int markerSize = 200; //tamanho da imagem

    int totalMarkers = 4;

    for (int id = 0; id < totalMarkers; id++) {
        Mat markerImage;

        //gera o marcador com o ID atual
        aruco::generateImageMarker(dictionary, id, markerSize, markerImage);

        string filename = "markers/marker_" + to_string(id) + ".png";

        imwrite(filename, markerImage); //salva na pasta

        cout << "Marcador " << id << " gerado: " << filename << endl;
    }

    cout << "Marcadores gerados" << endl;

    return 0;
}