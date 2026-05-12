#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "src/Camera.h"
#include "src/DetectorAR.h"
#include "src/MarcadorAruco.h"
#include "src/GerenciadorConteudo.h"
#include "src/ConfigLoader.h"

using namespace std;
using namespace cv;

// ============================================================
// Funcao principal - AR EDUCACIONAL.
// Cada marcador ArUco exibe um conteudo de uma materia diferente
// (Biologia, Historia, Geografia). Conteudos sao carregados de
// "conteudos.txt" -> amigos podem adicionar mais sem mexer no codigo.
// ESC sai.
// ============================================================
int main() {
    Camera camera(0);
    if (!camera.estaAberta()) {
        cout << "Erro ao abrir camera!!!!!!" << endl;
        return -1;
    }

    float ladoMarcador = 0.05f; // 5 cm
    DetectorAR detector(camera, ladoMarcador);

    // carrega todos os conteudos a partir do arquivo de config
    GerenciadorConteudo gerenciador;
    int n = ConfigLoader::carregar("conteudos.txt", gerenciador);
    cout << "Conteudos carregados: " << n << endl;
    cout << "Pressione ESC para sair." << endl;

    Mat frame;
    while (true) {
        if (!camera.capturarFrame(frame)) break;

        vector<MarcadorAruco> marcadores = detector.detectar(frame);
        detector.desenharMarcadores(frame, marcadores);

        for (const auto& m : marcadores) {
            drawFrameAxes(frame,
                          camera.getCameraMatrix(),
                          camera.getDistCoeffs(),
                          m.getRvec(), m.getTvec(),
                          ladoMarcador);

            // chamada polimorfica: o gerenciador escolhe o conteudo
            // certo conforme o ID detectado e ele se desenha sozinho.
            gerenciador.renderizarPara(m.getId(),
                                       frame,
                                       camera.getCameraMatrix(),
                                       camera.getDistCoeffs(),
                                       m.getRvec(), m.getTvec());
        }

        imshow("AR Educacional", frame);
        if (waitKey(1) == 27) break;
    }

    destroyAllWindows();
    return 0;
}
