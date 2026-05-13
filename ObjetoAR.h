#pragma once
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

// classe base abstrata pra todos os objetos AR do projeto
class ObjetoAR {

protected:
    int id;       // ID do marcador ArUco associado
    string nome;

public:
    ObjetoAR(int id, string nome) : id(id), nome(nome) {}

    // virtual pra garantir que o destrutor da subclasse seja chamado corretamente
    virtual ~ObjetoAR() {}

    // cada objeto implementa seu próprio render (polimorfismo)
    virtual void render(Mat& frame,
                        const Mat& cameraMatrix,
                        const Mat& distCoeffs,
                        const Vec3d& rvec,
                        const Vec3d& tvec) = 0;

    int getId() const { return id; }
    string getNome() const { return nome; }
};
