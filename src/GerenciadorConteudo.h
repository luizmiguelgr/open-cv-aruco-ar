#ifndef GERENCIADOR_CONTEUDO_H
#define GERENCIADOR_CONTEUDO_H

#include <map>
#include <opencv2/opencv.hpp>
#include "ConteudoEducacional.h"

// ============================================================
// GerenciadorConteudo - associa ID do marcador ArUco -> Conteudo.
// Encapsula o map e cuida da memoria (delete no destrutor).
// ============================================================
class GerenciadorConteudo {
private:
    std::map<int, ConteudoEducacional*> conteudos;

public:
    GerenciadorConteudo() = default;
    ~GerenciadorConteudo();

    // registra um conteudo para um id de marcador. Se ja existir, sobrescreve.
    void registrar(int idMarcador, ConteudoEducacional* conteudo);

    // retorna o conteudo associado ao id, ou nullptr se nao tiver.
    ConteudoEducacional* buscar(int idMarcador) const;

    // renderiza o conteudo certo pra um marcador detectado.
    void renderizarPara(int idMarcador,
                        cv::Mat& frame,
                        const cv::Mat& cameraMatrix,
                        const cv::Mat& distCoeffs,
                        const cv::Vec3d& rvec,
                        const cv::Vec3d& tvec) const;

    size_t quantidade() const { return conteudos.size(); }
};

#endif
