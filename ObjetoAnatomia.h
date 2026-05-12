#pragma once
#include "ObjetoAR.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Classe intermediária para objetos de anatomia
// Herda de ObjetoAR e serve de base para todos os modelos anatômicos
class ObjetoAnatomia : public ObjetoAR {

protected:
    // Caminho para o arquivo .obj do modelo 3D
    string caminhoModelo;

    // Importador do Assimp responsável por carregar o arquivo .obj
    Assimp::Importer importador;

    // Cena carregada com todos os dados do modelo 3D (vértices, faces, etc.)
    const aiScene* cena;

public:
    // Construtor: recebe o ID, nome e caminho do modelo
    ObjetoAnatomia(int id, string nome, string caminhoModelo)
        : ObjetoAR(id, nome), caminhoModelo(caminhoModelo) {

        // Carrega o modelo 3D ao criar o objeto
        cena = importador.ReadFile(caminhoModelo,
            aiProcess_Triangulate |    // converte faces para triângulos
            aiProcess_FlipUVs);        // corrige as coordenadas de textura

        if (!cena) {
            cout << "Erro ao carregar modelo: " << caminhoModelo << endl;
        } else {
            cout << "Modelo carregado: " << nome << endl;
        }
    }

    // Destrutor
    virtual ~ObjetoAnatomia() {}

    // render() ainda é virtual puro aqui
    // cada subclasse de anatomia vai implementar do seu jeito
    virtual void render(Mat& frame,
                        const Mat& cameraMatrix,
                        const Mat& distCoeffs,
                        const Vec3d& rvec,
                        const Vec3d& tvec) = 0;
};