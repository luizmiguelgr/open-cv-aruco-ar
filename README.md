# ArUco AR — Realidade Aumentada com OpenCV

Projeto da disciplina CIN0135 (Estruturas de Dados Orientadas a Objetos) — CIn/UFPE.

A aplicação detecta marcadores ArUco pela webcam e sobrepõe modelos 3D em wireframe sobre eles em tempo real.

---

## Configuração do ambiente

Veja o [SETUP.md](SETUP.md) para instalar as dependências (vcpkg, OpenCV, Assimp) e configurar o VS Code.

---

## Como compilar e rodar

Com o ambiente configurado, abra o projeto no VS Code e use o CMake Tools para compilar. Alternativamente, pela linha de comando:

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build . --config Release
```

Isso gera dois executáveis dentro de `build/Release/`:

| Executável | O que faz |
|---|---|
| `aruco_ar` | Aplicação principal (webcam + AR) |
| `generate_markers` | Gera os marcadores ArUco em PNG |

### Gerando os marcadores

Antes de rodar pela primeira vez, gere os marcadores que serão impressos ou exibidos na tela:

```powershell
.\build\Release\generate_markers.exe
```

Os arquivos `marker_0.png`, `marker_1.png` e `marker_2.png` serão criados na pasta `markers/`.

### Rodando a aplicação

```powershell
.\build\Release\aruco_ar.exe
```

A janela da webcam abre automaticamente. Aponte a câmera para um dos marcadores impressos e o modelo 3D correspondente aparece sobre ele. Pressione **ESC** para sair.

| Marcador | Modelo |
|---|---|
| `marker_0.png` | Torre Eiffel |
| `marker_1.png` | Estátua |
| `marker_2.png` | Crânio |

---

## Arquitetura

O projeto usa herança e polimorfismo para separar o que é comum (carregar e renderizar um modelo 3D) do que é específico de cada objeto.

```
ObjetoAR  (classe base abstrata)
│   render() → método virtual puro
│
├── ObjetoHistoria  (abstrata — carrega modelo via Assimp)
│   ├── TorreEiffelAR   (ID 0)
│   └── EstatuaAR       (ID 1)
│
└── ObjetoAnatomia  (abstrata — mesma estrutura de ObjetoHistoria)
    └── CranioAR        (ID 2)

GerenciadorAR  (mapeia ID do marcador → ObjetoAR*)
```

**ObjetoAR** define a interface: todo objeto AR tem um ID, um nome e um método `render()`. Como `render()` é virtual puro, cada subclasse concreta é obrigada a implementar a sua versão.

**ObjetoHistoria** e **ObjetoAnatomia** são classes intermediárias que cuidam do carregamento do arquivo `.obj` com o Assimp. As subclasses concretas herdam a cena carregada e só precisam implementar o `render()`.

**GerenciadorAR** mantém um `map<int, ObjetoAR*>` que associa cada ID de marcador ao objeto correspondente. O ponteiro para a classe base é o que permite chamar o `render()` correto de cada objeto sem o `main` precisar saber qual subclasse está sendo usada.

---

## Como funciona

O loop principal em `main.cpp` faz quatro coisas a cada frame:

1. **Detecta os marcadores** — `ArucoDetector` encontra os marcadores ArUco no frame e retorna os cantos de cada um e seus IDs.

2. **Estima a pose** — `solvePnP` recebe os cantos do marcador na imagem e os pontos equivalentes no espaço 3D real (baseado no tamanho físico do marcador em metros) e calcula os vetores de rotação (`rvec`) e translação (`tvec`) que descrevem onde o marcador está no espaço.

3. **Busca o objeto** — `GerenciadorAR::getObjeto(id)` retorna o ponteiro para o objeto 3D vinculado àquele ID.

4. **Renderiza** — chama `objeto->render(frame, rvec, tvec, ...)`, que projeta os vértices do modelo 3D no plano 2D da câmera com `projectPoints` e desenha as arestas de cada triângulo da malha.

O resultado é o wireframe do modelo "colado" sobre o marcador, acompanhando sua posição e rotação em tempo real.

---

## Dependências

- [OpenCV 4](https://opencv.org/) — detecção de marcadores ArUco e operações de visão computacional
- [Assimp](https://assimp.org/) — carregamento de modelos 3D (.obj)
- [vcpkg](https://vcpkg.io/) — gerenciador de pacotes C++
- CMake 3.15+
