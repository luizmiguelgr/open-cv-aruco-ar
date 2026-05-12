# Relatório do Projeto — AR Educacional com OpenCV e ArUco

**Disciplina:** Programação Orientada a Objetos (2º período)
**Tipo de projeto:** Tipo 2 — Aplicação com biblioteca de visão computacional (OpenCV)
**Linguagem:** C++17
**Bibliotecas:** OpenCV 4.x (módulos `core`, `imgproc`, `videoio`, `highgui`, `calib3d`, `objdetect/aruco`)

---

## 1. Descrição do projeto

O projeto é uma aplicação de **Realidade Aumentada Educacional**. A câmera do computador captura o vídeo em tempo real e detecta marcadores fiduciais do tipo **ArUco** impressos em papel. Sobre cada marcador detectado é renderizado um **modelo 3D em wireframe** associado a uma **matéria escolar** (Biologia, História, Geografia), junto com uma legenda na tela informando o nome da matéria e do conteúdo.

A motivação é demonstrar como conceitos de Programação Orientada a Objetos (herança, polimorfismo, composição, abstração) permitem construir um sistema **extensível**: qualquer integrante do grupo pode adicionar novos modelos 3D ou criar matérias adicionais sem precisar modificar o núcleo do código, apenas editando um arquivo de configuração externo (`conteudos.txt`) ou criando uma única subclasse nova.

### O que o sistema faz
1. Abre a webcam e capta vídeo em tempo real.
2. Detecta marcadores ArUco no frame e estima a pose 3D de cada um (`rvec`, `tvec`).
3. Lê o arquivo `conteudos.txt` no início e monta um mapa "ID do marcador → conteúdo educacional".
4. Para cada marcador detectado, projeta o modelo 3D correspondente em wireframe sobre o frame e desenha um HUD com a barra colorida da matéria e o título do conteúdo.
5. Suporta modelos `.obj` (Wavefront ASCII) com transformações locais (escala, rotação, offset) por modelo.

---

## 2. Arquitetura do sistema

O código está organizado em três camadas conceituais:

| Camada | Responsabilidade | Classes |
|---|---|---|
| **Infraestrutura** | Captura de vídeo e detecção ArUco | `Camera`, `MarcadorAruco`, `DetectorAR` |
| **Geometria** | Como desenhar formas 3D no frame | `Objeto3D` (abstrata), `Cubo`, `Piramide`, `ModeloOBJ`, `ParserOBJ` |
| **Educação** | Camada didática: matéria + conteúdo + HUD | `Materia`, `ConteudoEducacional` (abstrata), `ConteudoBiologia`, `ConteudoHistoria`, `ConteudoGeografia` |
| **Orquestração** | Liga marcadores a conteúdos e lê config externa | `GerenciadorConteudo`, `ConfigLoader` |

### 2.1 Diagrama de classes (textual)

```
                  ┌────────────────────┐
                  │   <<abstract>>     │
                  │     Objeto3D       │  ← classe base abstrata (interface)
                  │ + desenhar() = 0   │     método virtual puro
                  │ + getNome() = 0    │
                  └────────▲───────────┘
                           │ herança
        ┌──────────────────┼────────────────────┐
        │                  │                    │
   ┌─────────┐       ┌─────────┐         ┌──────────────┐
   │  Cubo   │       │ Piramide│         │  ModeloOBJ   │
   └─────────┘       └─────────┘         └──────────────┘
                                              │ usa
                                              ▼
                                        ┌──────────────┐
                                        │  ParserOBJ   │   (utilitário estático)
                                        │ ::carregar() │
                                        └──────────────┘

                  ┌────────────────────────┐
                  │     <<abstract>>       │
                  │  ConteudoEducacional   │
                  │ - materia: Materia     │
                  │ - titulo: string       │
                  │ - descricao: string    │
                  │ - modelo: Objeto3D*    │  ← composição
                  │ + renderizar()         │     (não é herança!)
                  │ + desenharHUD()        │
                  │ + getCuriosidade()=0   │
                  └──────────▲─────────────┘
                             │ herança
       ┌─────────────────────┼──────────────────────┐
       │                     │                      │
┌──────────────┐    ┌──────────────┐       ┌──────────────────┐
│ConteudoBiolog│    │ConteudoHistor│       │ConteudoGeografia │
└──────────────┘    └──────────────┘       └──────────────────┘

┌──────────────────────┐         ┌──────────────────────┐
│ GerenciadorConteudo  │ ◇────▶  │ ConteudoEducacional* │
│ -map<int,Conteudo*>  │         └──────────────────────┘
│ +registrar()         │
│ +buscar()            │
│ +renderizarPara()    │
└──────────────────────┘
         ▲
         │ popula
         │
┌──────────────────┐         ┌────────────────┐
│  ConfigLoader    │ ─────▶  │ conteudos.txt  │
│  ::carregar()    │   lê    │  (config       │
└──────────────────┘         │   externa)     │
                             └────────────────┘
```

### 2.2 Fluxo de execução

```
main()
  │
  ├─ instancia Camera(0) ─────────────► abre webcam
  ├─ instancia DetectorAR
  ├─ ConfigLoader::carregar("conteudos.txt", gerenciador)
  │      │
  │      └─ pra cada linha do .txt:
  │           ├─ criarObjeto() ──► Cubo | Piramide | ModeloOBJ
  │           └─ criarConteudo() ─► ConteudoBiologia | ...Historia | ...Geografia
  │
  └─ loop principal:
        ├─ camera.capturarFrame()
        ├─ detector.detectar(frame) ─► vector<MarcadorAruco>
        └─ pra cada marcador:
              └─ gerenciador.renderizarPara(id, ...)
                    └─ conteudo->renderizar(...)  [POLIMORFISMO]
                          ├─ modelo->desenhar(...) [POLIMORFISMO]
                          └─ desenharHUD(...)
```

---

## 3. Conceitos de POO utilizados

### 3.1 Classes e objetos
O sistema é totalmente baseado em classes. Cada arquivo `.h`/`.cpp` corresponde a uma única responsabilidade. Exemplos: `Camera` encapsula a captura de vídeo e os parâmetros intrínsecos; `MarcadorAruco` representa um marcador detectado com seus vetores de rotação e translação.

### 3.2 Herança
Duas hierarquias paralelas:

**(a) Hierarquia de geometria** — `Objeto3D` é classe base abstrata; `Cubo`, `Piramide` e `ModeloOBJ` herdam dela publicamente e implementam `desenhar()` e `getNome()`.

```cpp
class Cubo : public Objeto3D {
public:
    Cubo(float tamanho, cv::Scalar cor);
    void desenhar(cv::Mat& frame, ...) override;
    std::string getNome() const override;
};
```

**(b) Hierarquia educacional** — `ConteudoEducacional` é base abstrata; `ConteudoBiologia`, `ConteudoHistoria`, `ConteudoGeografia` herdam dela e sobrescrevem `getCuriosidade()`.

### 3.3 Polimorfismo
Usado em dois pontos críticos do sistema:

**Ponto 1 — desenho do objeto 3D.** O `ConteudoEducacional` guarda um `Objeto3D*` (ponteiro pra classe base). Em `renderizar()` ele chama `modelo->desenhar(...)`. O método executado em tempo de execução depende do tipo real (Cubo, Piramide ou ModeloOBJ) — resolução via vtable.

**Ponto 2 — escolha do conteúdo.** O `GerenciadorConteudo` guarda `map<int, ConteudoEducacional*>`. Quando o `main` chama `gerenciador.renderizarPara(id, ...)`, o gerenciador chama `conteudo->renderizar(...)` sem saber se é Biologia, História ou Geografia — cada subclasse pode customizar `getCuriosidade()` (e poderia sobrescrever `desenharHUD()` também).

### 3.4 Modificadores de acesso
- `private` para campos internos das classes (ex.: `Camera::cap`, `ModeloOBJ::malha`, `GerenciadorConteudo::conteudos`).
- `protected` para campos da base abstrata acessíveis pelas filhas (ex.: `Objeto3D::tamanho`, `Objeto3D::cor`, `ConteudoEducacional::modelo`).
- `public` apenas para a API exposta: construtores, getters, e métodos virtuais sobrescritos.

### 3.5 Composição vs. Herança
Decisão importante de design: `ConteudoEducacional` **contém** um `Objeto3D*` (composição), em vez de **ser** um `Objeto3D` (herança). Isso é coerente com a regra "um conteúdo educacional tem um objeto 3D, mas não é um objeto 3D". Permite combinar livremente qualquer matéria com qualquer geometria — Biologia pode usar Cubo, Pirâmide ou ModeloOBJ sem precisar de classes adicionais.

### 3.6 Abstração (classes abstratas)
`Objeto3D` declara `virtual void desenhar(...) = 0` e `virtual std::string getNome() const = 0` — métodos virtuais puros. Não dá pra instanciar `Objeto3D` diretamente; obriga toda subclasse a implementar. O mesmo vale para `ConteudoEducacional::getCuriosidade()`.

### 3.7 Encapsulamento
Acesso aos campos internos é sempre por getters (`getNome()`, `getCor()`, `getTitulo()`). Campos privados não são expostos diretamente, o que protege invariantes e permite mudar a implementação interna sem quebrar quem usa a classe.

### 3.8 Ponteiros e referências
- **Ponteiros para a classe base** (`Objeto3D*`, `ConteudoEducacional*`) habilitam o polimorfismo.
- **Destrutor virtual** em `Objeto3D` e `ConteudoEducacional` garante que `delete` chame o destrutor correto da subclasse (caso contrário haveria vazamento).
- **Referências `const`** em parâmetros pesados (`const cv::Mat&`, `const std::string&`) evitam cópias custosas mantendo segurança.
- `GerenciadorConteudo` é dono dos ponteiros que recebe — libera tudo no destrutor, evitando vazamento de memória.

### 3.9 Padrões de projeto aplicados (informalmente)

| Padrão | Onde aparece | Como |
|---|---|---|
| **Strategy** | `Objeto3D` | A "estratégia" de desenho varia (cubo, pirâmide, modelo OBJ) sem que o cliente saiba qual é. |
| **Factory** | `ConfigLoader::criarObjeto()` e `criarConteudo()` | Funções que recebem um tipo em texto ("cubo", "obj", "Biologia") e devolvem a instância correta. |
| **Template Method** (parcial) | `ConteudoEducacional::renderizar()` | Define o esqueleto (desenhar modelo + HUD) e delega passos variáveis (`getCuriosidade()`) às subclasses. |
| **Composite** (leve) | `GerenciadorConteudo` | Trata uma coleção de conteúdos com a mesma interface usada para um único. |

---

## 4. Uso do OpenCV (o que veio da biblioteca)

A biblioteca fornece a infraestrutura de visão computacional, **não** a lógica do sistema. As classes nossas usam OpenCV como ferramenta. Resumo:

| Recurso do OpenCV | Onde usamos | O que faz |
|---|---|---|
| `cv::VideoCapture` | `Camera` | Captura frames da webcam |
| `cv::Mat` | em todas as classes que desenham | Matriz que representa a imagem (frame) |
| `cv::aruco::ArucoDetector` | `DetectorAR` | Detecta marcadores fiduciais |
| `cv::solvePnP` (via aruco) | `DetectorAR` | Estima pose 3D do marcador (rvec, tvec) |
| `cv::projectPoints` | `Cubo`, `Piramide`, `ModeloOBJ` | Projeta vértices 3D no plano 2D da imagem |
| `cv::line`, `cv::rectangle`, `cv::putText` | desenho de wireframe e HUD | Primitivas de desenho 2D |
| `cv::drawFrameAxes` | `main` | Eixos coloridos sobre o marcador |

**O que é responsabilidade nossa (POO):**
- Encapsular `VideoCapture` numa classe `Camera` com construtor que abre a webcam e destrutor que libera o recurso (RAII).
- Hierarquia de objetos desenháveis (`Objeto3D` e filhas).
- Parser de `.obj` (`ParserOBJ`) — feito do zero, não usa loader do OpenCV.
- Toda a camada educacional (`Materia`, `ConteudoEducacional` e filhas, `GerenciadorConteudo`).
- Sistema de configuração externa (`ConfigLoader` + `conteudos.txt`).

---

## 5. Extensibilidade

Dois pontos planejados de extensão:

**(1) Adicionar um modelo novo** — sem código:
```
Geografia | 5 | obj | vulcao/vulcao.obj | 0.02 | 0,0,0 | 0,0,0 | 200,80,40 | Vulcão | Tipo de relevo
```
Adiciona linha no `conteudos.txt` e roda. Não recompila.

**(2) Adicionar uma matéria nova** — exige uma única subclasse:
1. Copiar `ConteudoGeografia.{h,cpp}` para `ConteudoArte.{h,cpp}` e trocar nome + cor.
2. Adicionar `if (materia == "Arte") return new ConteudoArte(...);` em `ConfigLoader::criarConteudo`.
3. Adicionar o `.cpp` no `CMakeLists.txt`.

---

## 6. Estrutura de arquivos

```
open-cv-aruco-ar/
├── main.cpp                    ← ponto de entrada
├── CMakeLists.txt              ← build com vcpkg + OpenCV
├── conteudos.txt               ← config externa dos conteúdos
├── generate_markers.cpp        ← utilitário pra gerar PNGs de ArUcos
├── markers/                    ← marcadores prontos pra imprimir
├── Skull/, M4A1/, rock/        ← modelos .obj
└── src/
    ├── Camera.{h,cpp}                  (infra)
    ├── MarcadorAruco.{h,cpp}           (infra)
    ├── DetectorAR.{h,cpp}              (infra)
    ├── Objeto3D.h                      (geometria - abstrata)
    ├── Cubo.{h,cpp}                    (geometria)
    ├── Piramide.{h,cpp}                (geometria)
    ├── ParserOBJ.{h,cpp}               (geometria - utilitário)
    ├── ModeloOBJ.{h,cpp}               (geometria)
    ├── Materia.h                       (educação)
    ├── ConteudoEducacional.{h,cpp}     (educação - abstrata)
    ├── ConteudoBiologia.{h,cpp}        (educação)
    ├── ConteudoHistoria.{h,cpp}        (educação)
    ├── ConteudoGeografia.{h,cpp}       (educação)
    ├── GerenciadorConteudo.{h,cpp}     (orquestração)
    └── ConfigLoader.{h,cpp}            (orquestração)
```

---

## 7. Como compilar e rodar

**Requisitos:** Windows, vcpkg em `C:/vcpkg`, OpenCV instalado via `vcpkg install opencv4`, CMake 3.15+.

```powershell
cmake -B build -S .
cmake --build build --config Release
.\build\Release\aruco_ar.exe
```

Imprima os marcadores ArUco 0 a 4 (gere com `generate_markers.exe`) e aponte a webcam para eles. Pressione **ESC** para sair.

---

## 8. Divisão do trabalho

> **Preencher antes da entrega.**

| Integrante | Contribuição |
|---|---|
| [Nome] | [...] |
| [Nome] | [...] |
| [Nome] | [...] |
| [Nome] | [...] |

---

## 9. Links

- **Código fonte (GitHub):** [URL]
- **Página GitHub Pages:** [URL]
- **Vídeo no YouTube:** [URL]
