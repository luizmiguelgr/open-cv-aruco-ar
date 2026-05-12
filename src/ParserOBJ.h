#ifndef PARSER_OBJ_H
#define PARSER_OBJ_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

// ============================================================
// Parser bem simples de arquivo .obj (Wavefront ASCII).
// So extrai vertices (v) e faces (f). Ignora vt, vn, mtllib.
// Faces com 4+ lados sao trianguladas em fan.
// ============================================================
struct MalhaOBJ {
    std::vector<cv::Point3f> vertices;
    std::vector<cv::Vec3i>   triangulos; // indices na lista de vertices
};

class ParserOBJ {
public:
    static bool carregar(const std::string& caminho, MalhaOBJ& malha);
};

#endif
