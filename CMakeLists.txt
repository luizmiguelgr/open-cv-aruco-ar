cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

project(aruco_ar)

find_package(OpenCV REQUIRED)
find_package(assimp REQUIRED)

add_executable(aruco_ar 
    main.cpp
    ObjetoAR.h
    ObjetoAnatomia.h
    ObjetoHistoria.h
    GerenciadorAR.h
)
target_link_libraries(aruco_ar ${OpenCV_LIBS} assimp::assimp)

add_executable(generate_markers generate_markers.cpp)
target_link_libraries(generate_markers ${OpenCV_LIBS})