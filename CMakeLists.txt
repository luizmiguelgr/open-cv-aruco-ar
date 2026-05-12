cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

project(aruco_ar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

# fontes da pasta src/ (uma classe por arquivo)
set(FONTES_SRC
    src/Camera.cpp
    src/MarcadorAruco.cpp
    src/DetectorAR.cpp
    src/Cubo.cpp
    src/Piramide.cpp
    src/ParserOBJ.cpp
    src/ModeloOBJ.cpp
    src/ConteudoEducacional.cpp
    src/ConteudoBiologia.cpp
    src/ConteudoHistoria.cpp
    src/ConteudoGeografia.cpp
    src/GerenciadorConteudo.cpp
    src/ConfigLoader.cpp
)

# executavel principal
add_executable(aruco_ar main.cpp ${FONTES_SRC})
target_include_directories(aruco_ar PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(aruco_ar ${OpenCV_LIBS})

# gerador de marcadores
add_executable(generate_markers generate_markers.cpp)
target_link_libraries(generate_markers ${OpenCV_LIBS})
