#ifndef MATERIA_H
#define MATERIA_H

#include <string>
#include <opencv2/opencv.hpp>

// ============================================================
// Materia escolar - so guarda nome + cor tema da disciplina.
// Cada ConteudoEducacional referencia uma Materia.
// ============================================================
class Materia {
private:
    std::string nome;
    cv::Scalar  corTema; // BGR

public:
    Materia() : nome("Generica"), corTema(255, 255, 255) {}
    Materia(const std::string& nome, cv::Scalar corTema)
        : nome(nome), corTema(corTema) {}

    std::string getNome()   const { return nome; }
    cv::Scalar  getCor()    const { return corTema; }
};

#endif
