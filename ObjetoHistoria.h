#pragma once
#include "ObjetoAR.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cfloat>

// base para objetos históricos, cuida do carregamento do modelo 3D
class ObjetoHistoria : public ObjetoAR {

protected:
    string caminhoModelo;
    Assimp::Importer importador;
    const aiScene* cena;

public:
    ObjetoHistoria(int id, string nome, string caminhoModelo)
        : ObjetoAR(id, nome), caminhoModelo(caminhoModelo) {

        cena = importador.ReadFile(caminhoModelo,
            aiProcess_Triangulate |  // converte polígonos em triângulos
            aiProcess_FlipUVs);      // corrige orientação das texturas

        if (!cena) {
            cout << "Erro ao carregar modelo: " << caminhoModelo << endl;
        }
    }

    virtual ~ObjetoHistoria() {}

    virtual void render(Mat& frame,
                        const Mat& cameraMatrix,
                        const Mat& distCoeffs,
                        const Vec3d& rvec,
                        const Vec3d& tvec) = 0;
};

// marcador ID 0
class TorreEiffelAR : public ObjetoHistoria {

public:
    TorreEiffelAR()
        : ObjetoHistoria(0, "Torre Eiffel", "assets/torreeiffel.obj") {}

    ~TorreEiffelAR() {}

    void render(Mat& frame,
                const Mat& cameraMatrix,
                const Mat& distCoeffs,
                const Vec3d& rvec,
                const Vec3d& tvec) override {

        if (!cena) return;

        // calcula bounding box pra centralizar e normalizar o modelo
        float minX = FLT_MAX, maxX = -FLT_MAX;
        float minY = FLT_MAX, maxY = -FLT_MAX;
        float minZ = FLT_MAX, maxZ = -FLT_MAX;

        for (unsigned int m = 0; m < cena->mNumMeshes; m++) {
            aiMesh* malha = cena->mMeshes[m];
            for (unsigned int v = 0; v < malha->mNumVertices; v++) {
                minX = min(minX, malha->mVertices[v].x);
                maxX = max(maxX, malha->mVertices[v].x);
                minY = min(minY, malha->mVertices[v].y);
                maxY = max(maxY, malha->mVertices[v].y);
                minZ = min(minZ, malha->mVertices[v].z);
                maxZ = max(maxZ, malha->mVertices[v].z);
            }
        }

        float cx = (minX + maxX) / 2.0f;
        float cy = (minY + maxY) / 2.0f;
        float cz = (minZ + maxZ) / 2.0f;

        float tamanho = max({maxX - minX, maxY - minY, maxZ - minZ});
        float escala = 0.15f / tamanho;

        for (unsigned int m = 0; m < cena->mNumMeshes; m++) {
            aiMesh* malha = cena->mMeshes[m];

            vector<Point3f> pontos3D;
            for (unsigned int v = 0; v < malha->mNumVertices; v++) {
                pontos3D.push_back(Point3f(
                    (malha->mVertices[v].x - cx) * escala,
                    (malha->mVertices[v].y - cy) * escala,
                    (malha->mVertices[v].z - cz) * escala + 0.05f
                ));
            }

            // projeta os pontos 3D no plano da câmera
            vector<Point2f> pontos2D;
            projectPoints(pontos3D, rvec, tvec,
                          cameraMatrix, distCoeffs, pontos2D);

            for (unsigned int f = 0; f < malha->mNumFaces; f++) {
                aiFace face = malha->mFaces[f];
                if (face.mNumIndices != 3) continue;

                line(frame, pontos2D[face.mIndices[0]], pontos2D[face.mIndices[1]], Scalar(0, 200, 255), 1);
                line(frame, pontos2D[face.mIndices[1]], pontos2D[face.mIndices[2]], Scalar(0, 200, 255), 1);
                line(frame, pontos2D[face.mIndices[2]], pontos2D[face.mIndices[0]], Scalar(0, 200, 255), 1);
            }
        }
    }
};

// marcador ID 1
class EstatuaAR : public ObjetoHistoria {

public:
    EstatuaAR()
        : ObjetoHistoria(1, "Estátua", "assets/statue.obj") {}

    ~EstatuaAR() {}

    void render(Mat& frame,
                const Mat& cameraMatrix,
                const Mat& distCoeffs,
                const Vec3d& rvec,
                const Vec3d& tvec) override {

        if (!cena) return;

        float minX = FLT_MAX, maxX = -FLT_MAX;
        float minY = FLT_MAX, maxY = -FLT_MAX;
        float minZ = FLT_MAX, maxZ = -FLT_MAX;

        for (unsigned int m = 0; m < cena->mNumMeshes; m++) {
            aiMesh* malha = cena->mMeshes[m];
            for (unsigned int v = 0; v < malha->mNumVertices; v++) {
                minX = min(minX, malha->mVertices[v].x);
                maxX = max(maxX, malha->mVertices[v].x);
                minY = min(minY, malha->mVertices[v].y);
                maxY = max(maxY, malha->mVertices[v].y);
                minZ = min(minZ, malha->mVertices[v].z);
                maxZ = max(maxZ, malha->mVertices[v].z);
            }
        }

        float cx = (minX + maxX) / 2.0f;
        float cy = (minY + maxY) / 2.0f;
        float cz = (minZ + maxZ) / 2.0f;

        float tamanho = max({maxX - minX, maxY - minY, maxZ - minZ});
        float escala = 0.15f / tamanho;

        for (unsigned int m = 0; m < cena->mNumMeshes; m++) {
            aiMesh* malha = cena->mMeshes[m];

            vector<Point3f> pontos3D;
            for (unsigned int v = 0; v < malha->mNumVertices; v++) {
                pontos3D.push_back(Point3f(
                    (malha->mVertices[v].x - cx) * escala,
                    (malha->mVertices[v].y - cy) * escala,
                    (malha->mVertices[v].z - cz) * escala + 0.05f
                ));
            }

            vector<Point2f> pontos2D;
            projectPoints(pontos3D, rvec, tvec,
                          cameraMatrix, distCoeffs, pontos2D);

            for (unsigned int f = 0; f < malha->mNumFaces; f++) {
                aiFace face = malha->mFaces[f];
                if (face.mNumIndices != 3) continue;

                line(frame, pontos2D[face.mIndices[0]], pontos2D[face.mIndices[1]], Scalar(0, 255, 255), 1);
                line(frame, pontos2D[face.mIndices[1]], pontos2D[face.mIndices[2]], Scalar(0, 255, 255), 1);
                line(frame, pontos2D[face.mIndices[2]], pontos2D[face.mIndices[0]], Scalar(0, 255, 255), 1);
            }
        }
    }
};
