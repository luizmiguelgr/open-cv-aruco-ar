#include "ParserOBJ.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace cv;

// le um indice no formato do .obj que pode ser:
//   "12"     -> so vertice
//   "12/3"   -> vertice/texcoord
//   "12/3/4" -> vertice/texcoord/normal
//   "12//4"  -> vertice//normal
// a gente quer SO o primeiro numero (indice do vertice).
// no .obj indices comecam em 1, entao subtraimos 1.
static int extrairIndiceVertice(const string& token) {
    int barra = token.find('/');
    string parte = (barra == (int)string::npos) ? token : token.substr(0, barra);
    return stoi(parte) - 1;
}

bool ParserOBJ::carregar(const string& caminho, MalhaOBJ& malha) {
    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        cerr << "ERRO: nao abriu arquivo OBJ -> " << caminho << endl;
        return false;
    }

    malha.vertices.clear();
    malha.triangulos.clear();

    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        istringstream ss(linha);
        string tipo;
        ss >> tipo;

        if (tipo == "v") {
            // vertice: v x y z
            float x, y, z;
            ss >> x >> y >> z;
            malha.vertices.push_back(Point3f(x, y, z));
        }
        else if (tipo == "f") {
            // face: f i j k  (ou mais lados)
            vector<int> idxs;
            string token;
            while (ss >> token) {
                idxs.push_back(extrairIndiceVertice(token));
            }
            // triangula em fan a partir do primeiro vertice
            for (size_t i = 1; i + 1 < idxs.size(); i++) {
                malha.triangulos.push_back(Vec3i(idxs[0], idxs[i], idxs[i+1]));
            }
        }
        // ignora vt, vn, mtllib, usemtl, g, s, o etc.
    }

    cout << "[OBJ] " << caminho
         << " -> " << malha.vertices.size() << " vertices, "
         << malha.triangulos.size() << " triangulos" << endl;
    return !malha.vertices.empty();
}
