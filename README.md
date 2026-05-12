# open-cv-aruco-ar
Projeto de Realidade Aumentada com ArUco Markers — CIn/UFPE
Disciplina: Estruturas de Dados Orientadas a Objetos (CIN0135)
Integrantes: Kaíque Bonfim, Luiz Miguel e Luiz Taiguara

## O que o projeto faz
Detecta marcadores ArUco pela câmera e renderiza modelos 3D em cima deles em tempo real.

- ID 0 -> Torre Eiffel (História)
- ID 1 -> Estátua (História)
- ID 2 -> Crânio (Anatomia)

## Requisitos
- Windows 10/11 64-bit
- Visual Studio Build Tools: https://visualstudio.microsoft.com/visual-cpp-build-tools/
  (marcar "Desenvolvimento para desktop com C++")
- Git: https://git-scm.com/download/win
- VS Code: https://code.visualstudio.com/
  Extensões: ms-vscode.cpptools, ms-vscode.cmake-tools, twxs.cmake

## Instalação

1. Instalar o vcpkg

cd C:\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

2. Instalar as bibliotecas

.\vcpkg install opencv4[contrib]:x64-windows
.\vcpkg install assimp:x64-windows

3. Clonar o repositório

git clone https://github.com/luizmiguelgr/open-cv-aruco-ar.git
cd open-cv-aruco-ar

4. Compilar

"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_PREFIX_PATH="C:/vcpkg/installed/x64-windows" -A x64 -S . -B build

"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build

5. Rodar

.\build\Debug\aruco_ar.exe

## Como usar
1. Imprima ou abra na tela os marcadores da pasta markers/
2. Rode o programa
3. Aponte a câmera para o marcador
4. O modelo 3D aparece em cima do marcador automaticamente
5. Pressione ESC para fechar