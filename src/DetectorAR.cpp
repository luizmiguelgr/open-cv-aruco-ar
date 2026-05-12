#include "DetectorAR.h"

using namespace std;
using namespace cv;

DetectorAR::DetectorAR(Camera& cam, float ladoMarcador)
    : camera(cam),
      ladoMarcador(ladoMarcador),
      dicionario(aruco::getPredefinedDictionary(aruco::DICT_4X4_50)),
      parametros(),
      detector(dicionario, parametros) {}

vector<MarcadorAruco> DetectorAR::detectar(const Mat& frame) {
    vector<vector<Point2f>> cantos;
    vector<int> ids;
    vector<vector<Point2f>> rejeitados;

    detector.detectMarkers(frame, cantos, ids, rejeitados);

    vector<MarcadorAruco> marcadores;
    for (int i = 0; i < (int)ids.size(); i++) {
        MarcadorAruco m(ids[i], cantos[i]);
        m.estimarPose(ladoMarcador,
                      camera.getCameraMatrix(),
                      camera.getDistCoeffs());
        marcadores.push_back(m);
    }
    return marcadores;
}

void DetectorAR::desenharMarcadores(Mat& frame,
                                    const vector<MarcadorAruco>& marcadores) {
    vector<vector<Point2f>> cantos;
    vector<int> ids;
    for (const auto& m : marcadores) {
        cantos.push_back(m.getCantos());
        ids.push_back(m.getId());
    }
    if (!ids.empty()) {
        aruco::drawDetectedMarkers(frame, cantos, ids);
    }
}
