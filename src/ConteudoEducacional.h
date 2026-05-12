#ifndef CONTEUDO_EDUCACIONAL_H
#define CONTEUDO_EDUCACIONAL_H

#include <string>
#include <opencv2/opencv.hpp>
#include "Objeto3D.h"
#include "Materia.h"

// ============================================================
// ConteudoEducacional - classe base abstrata da camada didatica.
// Cada conteudo COMPOE um Objeto3D (geometria) e ASSOCIA uma
// Materia (cor/tema). Subclasses (Biologia, Historia, Geografia)
// podem sobrescrever getCuriosidade() pra texto extra.
// ============================================================
class ConteudoEducacional {
protected:
    Materia      materia;
    std::string  titulo;
    std::string  descricao;
    Objeto3D*    modelo; // composicao: ponteiro pra geometria

public:
    ConteudoEducacional(Materia materia,
                        const std::string& titulo,
                        const std::string& descricao,
                        Objeto3D* modelo)
        : materia(materia), titulo(titulo), descricao(descricao), modelo(modelo) {}

    virtual ~ConteudoEducacional() {
        delete modelo; // dono do ponteiro
    }

    // desenha o modelo 3d + HUD com info da materia
    virtual void renderizar(cv::Mat& frame,
                            const cv::Mat& cameraMatrix,
                            const cv::Mat& distCoeffs,
                            const cv::Vec3d& rvec,
                            const cv::Vec3d& tvec);

    // HUD simples: barra no topo "MATERIA - Titulo"
    virtual void desenharHUD(cv::Mat& frame) const;

    // curiosidade extra - cada materia pode personalizar
    virtual std::string getCuriosidade() const { return descricao; }

    // getters
    const Materia&    getMateria()   const { return materia; }
    const std::string& getTitulo()   const { return titulo; }
    const std::string& getDescricao() const { return descricao; }
};

#endif
