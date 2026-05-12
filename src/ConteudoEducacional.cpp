#include "ConteudoEducacional.h"

using namespace std;
using namespace cv;

void ConteudoEducacional::renderizar(Mat& frame,
                                     const Mat& cameraMatrix,
                                     const Mat& distCoeffs,
                                     const Vec3d& rvec,
                                     const Vec3d& tvec) {
    if (modelo) {
        modelo->desenhar(frame, cameraMatrix, distCoeffs, rvec, tvec);
    }
    desenharHUD(frame);
}

void ConteudoEducacional::desenharHUD(Mat& frame) const {
    // barra colorida no topo
    int largura = frame.cols;
    rectangle(frame, Point(0, 0), Point(largura, 60),
              materia.getCor(), FILLED);

    // texto "MATERIA"
    putText(frame, materia.getNome(),
            Point(15, 25), FONT_HERSHEY_SIMPLEX,
            0.7, Scalar(255, 255, 255), 2);

    // texto "Titulo"
    putText(frame, titulo,
            Point(15, 50), FONT_HERSHEY_SIMPLEX,
            0.6, Scalar(255, 255, 255), 1);

    // curiosidade no rodape
    string txt = getCuriosidade();
    if (!txt.empty()) {
        int yBase = frame.rows - 15;
        rectangle(frame, Point(0, yBase - 25), Point(largura, frame.rows),
                  Scalar(0, 0, 0), FILLED);
        putText(frame, txt,
                Point(15, yBase - 5), FONT_HERSHEY_SIMPLEX,
                0.5, Scalar(255, 255, 255), 1);
    }
}
