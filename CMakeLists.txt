cmake_minimum_required(VERSION 3.15)
project(aruco_ar)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(OpenCV REQUIRED)

add_executable(aruco_ar main.cpp)

target_link_libraries(aruco_ar ${OpenCV_LIBS})