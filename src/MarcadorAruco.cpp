#include "MarcadorAruco.h"

using namespace std;
using namespace cv;

MarcadorAruco::MarcadorAruco(int id, const vector<Point2f>& cantos) {
    this->id = id;
    this->cantos = cantos;
}

void MarcadorAruco::estimarPose(float ladoMarcador,
                                const Mat& cameraMatrix,
                                const Mat& distCoeffs) {
    // os 4 cantos do marcador no mundo real (em metros),
    // com origem no centro do marcador
    float meio = ladoMarcador / 2.0f;
    vector<Point3f> pontosObjeto = {
        {-meio,  meio, 0},  // sup esquerdo
        { meio,  meio, 0},  // sup direito
        { meio, -meio, 0},  // inf direito
        {-meio, -meio, 0}   // inf esquerdo
    };

    solvePnP(pontosObjeto, cantos, cameraMatrix, distCoeffs, rvec, tvec);
}
