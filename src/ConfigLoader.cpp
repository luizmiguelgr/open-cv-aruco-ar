#include "ConfigLoader.h"
#include "Cubo.h"
#include "Piramide.h"
#include "ModeloOBJ.h"
#include "ConteudoBiologia.h"
#include "ConteudoHistoria.h"
#include "ConteudoGeografia.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace cv;

// tira espacos do comeco e do fim
static string trim(const string& s) {
    size_t i = s.find_first_not_of(" \t\r\n");
    size_t j = s.find_last_not_of(" \t\r\n");
    if (i == string::npos) return "";
    return s.substr(i, j - i + 1);
}

// quebra "a|b|c" em ["a","b","c"]
static vector<string> split(const string& s, char sep) {
    vector<string> out;
    string item;
    istringstream ss(s);
    while (getline(ss, item, sep)) {
        out.push_back(trim(item));
    }
    return out;
}

// quebra "1.0,2.0,3.0" em Vec3f
static Vec3f parseVec3(const string& s) {
    auto p = split(s, ',');
    if (p.size() < 3) return Vec3f(0, 0, 0);
    return Vec3f(stof(p[0]), stof(p[1]), stof(p[2]));
}

static Scalar parseCor(const string& s) {
    auto p = split(s, ',');
    if (p.size() < 3) return Scalar(255, 255, 255);
    // arquivo em R,G,B -> OpenCV usa BGR
    return Scalar(stoi(p[2]), stoi(p[1]), stoi(p[0]));
}

// cria o Objeto3D da linha conforme o tipo
static Objeto3D* criarObjeto(const string& tipo, const string& arquivo,
                             float escala, Vec3f rot, Vec3f off, Scalar cor,
                             const string& titulo) {
    if (tipo == "cubo")     return new Cubo(escala, cor);
    if (tipo == "piramide") return new Piramide(escala, cor);
    if (tipo == "obj")      return new ModeloOBJ(arquivo, titulo, escala, rot, off, cor);
    cerr << "tipo desconhecido: " << tipo << endl;
    return nullptr;
}

// cria o ConteudoEducacional conforme a materia
static ConteudoEducacional* criarConteudo(const string& materia,
                                          const string& titulo,
                                          const string& descricao,
                                          Objeto3D* modelo) {
    if (materia == "Biologia")  return new ConteudoBiologia(titulo, descricao, modelo);
    if (materia == "Historia")  return new ConteudoHistoria(titulo, descricao, modelo);
    if (materia == "Geografia") return new ConteudoGeografia(titulo, descricao, modelo);
    cerr << "materia desconhecida: " << materia << endl;
    delete modelo;
    return nullptr;
}

int ConfigLoader::carregar(const string& caminho, GerenciadorConteudo& g) {
    ifstream arq(caminho);
    if (!arq.is_open()) {
        cerr << "ERRO: nao abriu config -> " << caminho << endl;
        return 0;
    }

    int total = 0;
    string linha;
    int nLinha = 0;
    while (getline(arq, linha)) {
        nLinha++;
        string t = trim(linha);
        if (t.empty() || t[0] == '#') continue;

        auto campos = split(t, '|');
        if (campos.size() < 10) {
            cerr << "linha " << nLinha << " invalida (campos faltando)" << endl;
            continue;
        }

        try {
            string materia  = campos[0];
            int    id       = stoi(campos[1]);
            string tipo     = campos[2];
            string arquivo  = campos[3];
            float  escala   = stof(campos[4]);
            Vec3f  rot      = parseVec3(campos[5]);
            Vec3f  off      = parseVec3(campos[6]);
            Scalar cor      = parseCor(campos[7]);
            string titulo   = campos[8];
            string descricao = campos[9];

            Objeto3D* obj = criarObjeto(tipo, arquivo, escala, rot, off, cor, titulo);
            if (!obj) continue;

            ConteudoEducacional* c = criarConteudo(materia, titulo, descricao, obj);
            if (!c) continue;

            g.registrar(id, c);
            total++;
            cout << "[CONFIG] id " << id << " -> " << materia
                 << " / " << titulo << endl;
        }
        catch (exception& e) {
            cerr << "erro na linha " << nLinha << ": " << e.what() << endl;
        }
    }
    return total;
}
