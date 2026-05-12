#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

// ============================================================
// Classe base abstrata - representa qualquer objeto 3D que a
// gente queira desenhar em cima do marcador ArUco.
// Toda subclasse vai precisar implementar o metodo desenhar().
// ============================================================
class Objeto3D {
protected:
    float tamanho;      // tamanho do objeto em metros
    Scalar cor;         // cor das linhas (BGR no OpenCV)

public:
    Objeto3D(float tamanho, Scalar cor) {
        this->tamanho = tamanho;
        this->cor = cor;
    }

    // destrutor virtual pq tem heranca
    virtual ~Objeto3D() {}

    // metodo virtual puro - obriga as filhas a implementarem
    virtual void desenhar(Mat& frame,
                          const Mat& cameraMatrix,
                          const Mat& distCoeffs,
                          const Vec3d& rvec,
                          const Vec3d& tvec) = 0;

    // getters basicos
    float getTamanho() const { return tamanho; }
    Scalar getCor() const { return cor; }
};


// ============================================================
// Classe Cubo - herda de Objeto3D e desenha um cubo 3d
// projetando os 8 vertices na tela usando projectPoints.
// ============================================================
class Cubo : public Objeto3D {
public:
    // chama o construtor da classe pai
    Cubo(float tamanho, Scalar cor) : Objeto3D(tamanho, cor) {}

    // override do metodo desenhar
    void desenhar(Mat& frame,
                  const Mat& cameraMatrix,
                  const Mat& distCoeffs,
                  const Vec3d& rvec,
                  const Vec3d& tvec) override {

        float s = tamanho;

        // 8 vertices do cubo no espaco 3d
        vector<Point3f> verticesCubo = {
            {-s, -s, 0},   {s, -s, 0},   {s, s, 0},   {-s, s, 0},      // base
            {-s, -s, -s*2},{s, -s, -s*2},{s, s, -s*2},{-s, s, -s*2}    // topo
        };

        // projeta os pontos 3d no plano 2d da camera
        vector<Point2f> pontosImagem;
        projectPoints(verticesCubo, rvec, tvec,
                      cameraMatrix, distCoeffs, pontosImagem);

        // desenha as arestas da base (0-1, 1-2, 2-3, 3-0)
        for (int i = 0; i < 4; i++) {
            line(frame, pontosImagem[i], pontosImagem[(i+1) % 4], cor, 2);
        }

        // desenha as arestas do topo (4-5, 5-6, 6-7, 7-4)
        for (int i = 0; i < 4; i++) {
            line(frame, pontosImagem[i+4], pontosImagem[(i+1) % 4 + 4], cor, 2);
        }

        // desenha as 4 arestas laterais ligando base e topo
        for (int i = 0; i < 4; i++) {
            line(frame, pontosImagem[i], pontosImagem[i+4], cor, 2);
        }
    }
};


// ============================================================
// Classe Camera - encapsula a webcam e os parametros intrinsecos
// (matriz da camera + coeficientes de distorcao).
// Aqui usamos uma aproximacao sem calibracao.
// ============================================================
class Camera {
private:
    VideoCapture cap;
    Mat cameraMatrix;
    Mat distCoeffs;

public:
    Camera(int idCamera = 0) {
        cap.open(idCamera);

        // matriz padrao - fx,fy = distancia focal, cx,cy = centro
        cameraMatrix = (Mat_<double>(3,3) <<
            800,   0, 320,
              0, 800, 240,
              0,   0,   1);

        // sem calibracao = distorcao zerada
        distCoeffs = Mat::zeros(5, 1, CV_64F);
    }

    ~Camera() {
        cap.release();
    }

    bool estaAberta() const {
        return cap.isOpened();
    }

    // pega o proximo frame da camera
    bool capturarFrame(Mat& frame) {
        cap >> frame;
        return !frame.empty();
    }

    const Mat& getCameraMatrix() const { return cameraMatrix; }
    const Mat& getDistCoeffs() const { return distCoeffs; }
};


// ============================================================
// Classe MarcadorAruco - representa um marcador ja detectado,
// guardando id, cantos na imagem e a pose estimada (rvec/tvec).
// ============================================================
class MarcadorAruco {
private:
    int id;
    vector<Point2f> cantos;
    Vec3d rvec;   // rotacao
    Vec3d tvec;   // translacao

public:
    MarcadorAruco(int id, const vector<Point2f>& cantos) {
        this->id = id;
        this->cantos = cantos;
    }

    // estima a pose do marcador usando solvePnP
    void estimarPose(float ladoMarcador,
                     const Mat& cameraMatrix,
                     const Mat& distCoeffs) {

        // os 4 cantos do marcador no mundo real (em metros),
        // com origem no centro do marcador
        float meio = ladoMarcador / 2.0f;
        vector<Point3f> pontosObjeto = {
            {-meio,  meio, 0},  // sup esquerdo
            { meio,  meio, 0},  // sup direito
            { meio, -meio, 0},  // inf direito
            {-meio, -meio, 0}   // inf esquerdo
        };

        solvePnP(pontosObjeto, cantos, cameraMatrix, distCoeffs, rvec, tvec);
    }

    int getId() const { return id; }
    const Vec3d& getRvec() const { return rvec; }
    const Vec3d& getTvec() const { return tvec; }
    const vector<Point2f>& getCantos() const { return cantos; }
};


// ============================================================
// Classe DetectorAR - junta tudo: usa a Camera, roda o detector
// do ArUco e devolve uma lista de MarcadorAruco com pose pronta.
// ============================================================
class DetectorAR {
private:
    Camera& camera;                       // referencia pra camera
    float ladoMarcador;                   // tamanho do marcador em metros
    aruco::Dictionary dicionario;
    aruco::DetectorParameters parametros;
    aruco::ArucoDetector detector;

public:
    DetectorAR(Camera& cam, float ladoMarcador)
        : camera(cam),
          ladoMarcador(ladoMarcador),
          dicionario(aruco::getPredefinedDictionary(aruco::DICT_4X4_50)),
          parametros(),
          detector(dicionario, parametros) {}

    // detecta marcadores no frame e ja estima a pose de cada um
    vector<MarcadorAruco> detectar(const Mat& frame) {
        vector<vector<Point2f>> cantos;
        vector<int> ids;
        vector<vector<Point2f>> rejeitados;

        detector.detectMarkers(frame, cantos, ids, rejeitados);

        vector<MarcadorAruco> marcadores;
        for (int i = 0; i < (int)ids.size(); i++) {
            MarcadorAruco m(ids[i], cantos[i]);
            m.estimarPose(ladoMarcador,
                          camera.getCameraMatrix(),
                          camera.getDistCoeffs());
            marcadores.push_back(m);
        }
        return marcadores;
    }

    // desenha contorno dos marcadores detectados (helper)
    void desenharMarcadores(Mat& frame, const vector<MarcadorAruco>& marcadores) {
        vector<vector<Point2f>> cantos;
        vector<int> ids;
        for (const auto& m : marcadores) {
            cantos.push_back(m.getCantos());
            ids.push_back(m.getId());
        }
        if (!ids.empty()) {
            aruco::drawDetectedMarkers(frame, cantos, ids);
        }
    }

    float getLadoMarcador() const { return ladoMarcador; }
};


// ============================================================
// Funcao principal - monta os objetos e roda o loop da camera.
// ============================================================
int main() {
    Camera camera(0);
    if (!camera.estaAberta()) {
        cout << "Erro ao abrir camera!!!!!!" << endl;
        return -1;
    }

    float ladoMarcador = 0.05f; // 5 cm
    DetectorAR detector(camera, ladoMarcador);

    // cria o objeto 3d que vai ser desenhado em cima do marcador.
    // como Objeto3D eh polimorfico, da pra trocar facil por outra
    // classe filha no futuro (ex: Piramide, Joelho, etc).
    Objeto3D* objeto = new Cubo(ladoMarcador / 2.0f, Scalar(0, 255, 0));

    Mat frame;
    while (true) {
        if (!camera.capturarFrame(frame)) break;

        vector<MarcadorAruco> marcadores = detector.detectar(frame);
        detector.desenharMarcadores(frame, marcadores);

        // pra cada marcador detectado desenha eixos + o objeto 3d
        for (const auto& m : marcadores) {
            drawFrameAxes(frame,
                          camera.getCameraMatrix(),
                          camera.getDistCoeffs(),
                          m.getRvec(), m.getTvec(),
                          ladoMarcador);

            objeto->desenhar(frame,
                             camera.getCameraMatrix(),
                             camera.getDistCoeffs(),
                             m.getRvec(), m.getTvec());

            cout << "ID: " << m.getId()
                 << " /// tvec: " << m.getTvec() << endl;
        }

        imshow("ArUco Detector", frame);

        if (waitKey(1) == 27) break; // ESC sai
    }

    delete objeto;             // libera memoria do new
    destroyAllWindows();
    return 0;
}
