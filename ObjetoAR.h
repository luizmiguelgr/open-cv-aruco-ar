#pragma once
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

// Classe base abstrata
// Todo objeto AR do projeto vai herdar dessa classe
class ObjetoAR {

protected:
    // Atributos protegidos: acessíveis pela classe e suas subclasses
    int id;       // ID do marcador ArUco (0, 1, 2, 3...)
    string nome;  // nome do objeto (ex: "Torre Eiffel")

public:
    // Construtor: inicializa id e nome quando o objeto é criado
    ObjetoAR(int id, string nome) : id(id), nome(nome) {}

    // Destrutor virtual: garante que a memória seja liberada corretamente
    // quando deletamos um objeto pelo ponteiro da classe base
    virtual ~ObjetoAR() {}

    // Método virtual puro: obriga todas as subclasses a implementarem render()
    // cada subclasse vai renderizar o seu modelo 3D do seu jeito (polimorfismo)
    virtual void render(Mat& frame,           // frame da câmera onde vai desenhar
                        const Mat& cameraMatrix,  // matriz da câmera
                        const Mat& distCoeffs,    // distorções da câmera
                        const Vec3d& rvec,        // rotação do marcador
                        const Vec3d& tvec) = 0;   // posição do marcador

    // Getter: retorna o ID do marcador
    int getId() const { return id; }

    // Getter: retorna o nome do objeto
    string getNome() const { return nome; }
};