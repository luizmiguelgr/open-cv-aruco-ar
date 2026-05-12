# AR Educacional — OpenCV + ArUco

Projeto de Programação Orientada a Objetos — CIn/UFPE (2º período).

Aplicação de Realidade Aumentada em C++ que detecta marcadores ArUco pela webcam e desenha modelos 3D de matérias escolares (Biologia, História, Geografia) sobre eles.

**Bibliotecas:** OpenCV 4.x (`core`, `imgproc`, `videoio`, `calib3d`, `objdetect/aruco`)

---

## Sumário rápido

- **`main.cpp`** — ponto de entrada, loop da câmera.
- **`src/`** — todas as classes (geometria + educação + orquestração).
- **`conteudos.txt`** — config externa: associa ID de marcador → conteúdo educacional. Editar este arquivo NÃO requer recompilar.
- **`markers/`** — PNGs dos marcadores ArUco prontos pra imprimir.
- **`Skull/`, `M4A1/`, `rock/`** — modelos `.obj`.
- **`RELATORIO.md`** — relatório completo do projeto.
- **`docs/index.html`** — página GitHub Pages.

## Compilar

Requisitos: Windows, vcpkg em `C:/vcpkg`, OpenCV (`vcpkg install opencv4`), CMake 3.15+.

```powershell
cmake -B build -S .
cmake --build build --config Release
.\build\Release\aruco_ar.exe
```

Pressione **ESC** para sair.

## Como funciona

1. Lê `conteudos.txt` e monta um `map<int, ConteudoEducacional*>`.
2. Detecta marcadores ArUco no vídeo da webcam.
3. Para cada marcador detectado, chama `gerenciador.renderizarPara(id, ...)`.
4. O conteúdo correspondente se desenha sozinho (modelo 3D em wireframe + HUD).

## Adicionar um modelo novo

1. Baixe um `.obj` ASCII low-poly.
2. Coloque numa subpasta do projeto.
3. Adicione uma linha em `conteudos.txt`:
   ```
   Geografia | 5 | obj | pasta/modelo.obj | 0.02 | 0,0,0 | 0,0,0 | 200,80,40 | Título | Descrição
   ```
4. Rode novamente — sem recompilar.

## Adicionar uma matéria nova

1. Copie `src/ConteudoGeografia.{h,cpp}` para `src/ConteudoArte.{h,cpp}` e mude nome + cor.
2. Em `src/ConfigLoader.cpp`, adicione um `if (materia == "Arte") return new ConteudoArte(...);`.
3. Adicione `src/ConteudoArte.cpp` em `CMakeLists.txt`.
4. Use `Arte` no `conteudos.txt`.

## Conceitos de POO aplicados

Ver [RELATORIO.md](RELATORIO.md) seção 3 para detalhamento. Resumo:

- **Herança:** `Objeto3D` → `Cubo`/`Piramide`/`ModeloOBJ`. `ConteudoEducacional` → `ConteudoBiologia`/`Historia`/`Geografia`.
- **Polimorfismo:** chamadas via ponteiro pra classe base resolvem o método certo em runtime.
- **Composição:** `ConteudoEducacional` contém um `Objeto3D*`, não herda dele.
- **Abstração:** métodos virtuais puros.
- **Encapsulamento:** campos privados, getters públicos.
- **Padrões:** Strategy, Factory, Template Method.

## Estrutura de arquivos

```
src/
├── Camera.{h,cpp}                  infra: webcam
├── MarcadorAruco.{h,cpp}           infra: representa marcador detectado
├── DetectorAR.{h,cpp}              infra: detecta ArUcos no frame
├── Objeto3D.h                      geometria: base abstrata
├── Cubo.{h,cpp}                    geometria
├── Piramide.{h,cpp}                geometria
├── ParserOBJ.{h,cpp}               geometria: lê .obj ASCII
├── ModeloOBJ.{h,cpp}               geometria: desenha modelo .obj
├── Materia.h                       educação: nome + cor
├── ConteudoEducacional.{h,cpp}     educação: base abstrata + HUD
├── ConteudoBiologia.{h,cpp}        educação
├── ConteudoHistoria.{h,cpp}        educação
├── ConteudoGeografia.{h,cpp}       educação
├── GerenciadorConteudo.{h,cpp}     orquestração: map ID → conteúdo
└── ConfigLoader.{h,cpp}            orquestração: lê conteudos.txt
```

## Equipe

Ver seção "Divisão do trabalho" no [RELATORIO.md](RELATORIO.md).
