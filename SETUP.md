# Como configurar o ambiente

## Requisitos
- Windows 10/11 64-bit
- [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) com "Desenvolvimento para desktop com C++"
- [Git](https://git-scm.com/download/win)
- [VS Code](https://code.visualstudio.com/) com extensões:
  - `ms-vscode.cpptools`
  - `ms-vscode.cmake-tools`
  - `twxs.cmake`

## Instalação do vcpkg

```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

## Instalação das bibliotecas

```powershell
.\vcpkg install opencv4[contrib]:x64-windows
.\vcpkg install assimp:x64-windows
```

## Clonar o repositório

```powershell
git clone https://github.com/luizmiguelgr/open-cv-aruco-ar.git
cd open-cv-aruco-ar
```

## Compilar