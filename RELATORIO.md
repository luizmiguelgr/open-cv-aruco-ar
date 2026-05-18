# ArUco AR — Realidade Aumentada com OpenCV

**Disciplina:** CIN0135 — Estruturas de Dados Orientadas a Objetos
**Instituição:** CIn/UFPE
**Tipo de projeto:** Tipo 2 (aplicação baseada em biblioteca — OpenCV + Assimp)

**Integrantes:** Luiz Taiguara · Luiz Miguel · Kaique Bonfim

---

## 1. O projeto

A gente queria fazer algo de Realidade Aumentada que rodasse com webcam comum, sem GPU, e que servisse pra mostrar herança e polimorfismo de um jeito que fizesse sentido (não só "exemplo de Animal e Cachorro").

O resultado foi o **ArUco AR**: uma aplicação em C++ que detecta marcadores ArUco impressos pela webcam e desenha modelos 3D em wireframe sobre eles em tempo real. Cada marcador é um modelo diferente:

| Marcador | Modelo       | Categoria  |
|----------|--------------|------------|
| ID 0     | Torre Eiffel | História   |
| ID 1     | Estátua      | História   |
| ID 2     | Crânio       | Anatomia   |

Quando a câmera enxerga um marcador, o programa calcula onde ele está no espaço 3D e "cola" o modelo correspondente em cima, acompanhando movimentos e rotações.

---

## 2. Arquitetura

A ideia central foi separar o **contrato comum** (todo objeto AR tem um ID, um nome e sabe se renderizar) do **comportamento específico** de cada modelo.

A hierarquia ficou assim:

```
              ObjetoAR  (abstrata)
                  │
        ┌─────────┴─────────┐
        │                   │
  ObjetoHistoria      ObjetoAnatomia
        │                   │
   ┌────┴─────┐             │
TorreEiffelAR EstatuaAR  CranioAR
   (ID 0)      (ID 1)    (ID 2)
```

E o `GerenciadorAR` por fora, que mantém um `map<int, ObjetoAR*>` mapeando o ID de cada marcador para o objeto correspondente.

### O que cada classe faz

- **`ObjetoAR`** — classe base abstrata. Tem `id`, `nome` e um `render()` virtual puro. É o "contrato": qualquer subclasse precisa implementar o render.
- **`ObjetoHistoria` / `ObjetoAnatomia`** — classes intermediárias abstratas. Cuidam de carregar o arquivo `.obj` com o Assimp para não repetir esse código em cada modelo. Continuam abstratas porque deixam o `render()` por conta das filhas.
- **`TorreEiffelAR`, `EstatuaAR`, `CranioAR`** — classes concretas. Cada uma implementa o seu `render()` (com cores e estilos um pouco diferentes).
- **`GerenciadorAR`** — guarda os objetos num `map` e expõe `getObjeto(id)`. É ele que sabe qual ID vai pra qual modelo.

### Fluxo a cada frame

```
Webcam → detectMarkers → solvePnP (estima pose 3D)
       → gerenciador.getObjeto(id) → objeto->render(...)
       → mostra o frame
```

---

## 3. O que é OO nosso × o que vem da biblioteca

Como o projeto é Tipo 2, vale separar bem essas duas coisas.

### O que vem pronto das bibliotecas

- **OpenCV**: a captura de vídeo (`VideoCapture`), a detecção dos marcadores (`ArucoDetector`), o cálculo da pose (`solvePnP`), a projeção 3D→2D (`projectPoints`) e o desenho de linhas (`line`). A gente só usa, não estende nada.
- **Assimp**: o carregamento do `.obj` (`Importer::ReadFile`) e as estruturas de mesh (`aiScene`, `aiMesh`, `aiFace`). Também só usamos.

### O que é OO autoral

Toda a hierarquia `ObjetoAR → ObjetoHistoria/ObjetoAnatomia → classes concretas` e o `GerenciadorAR` foram desenhados pela gente. É aí que aparece o conteúdo da disciplina:

- **Classe abstrata** — `ObjetoAR` tem `render() = 0`, então não dá pra instanciar direto. Toda subclasse é obrigada a implementar.
- **Herança em dois níveis** — em vez de cada modelo herdar direto de `ObjetoAR`, criamos uma camada intermediária pra evitar duplicar o carregamento do Assimp.
- **Polimorfismo** — o `main` só conhece `ObjetoAR*`. Quando faz `objeto->render(...)`, é o C++ que decide em runtime se chama o render da Torre, da Estátua ou do Crânio. Isso aparece bem claro em `main.cpp`:

```cpp
ObjetoAR* objeto = gerenciador.getObjeto(ids[i]);
if (objeto != nullptr) {
    objeto->render(frame, cameraMatrix, distCoeffs, rvec, tvec);
}
```

- **Destrutor virtual** — `virtual ~ObjetoAR()`. Sem isso, quando o `GerenciadorAR` faz `delete` no ponteiro de base, o destrutor da subclasse não roda e o Assimp vaza.
- **Encapsulamento** — atributos `protected`/`private`, acesso por `getId()` / `getNome()`.
- **Composição** — `GerenciadorAR` é dono dos objetos: cria no construtor, libera no destrutor.

> Sobre as duas classes intermediárias terem código parecido: a separação foi por **significado** (objetos históricos × anatômicos), não por necessidade técnica. Deixa espaço claro pra evoluir cada categoria por conta própria — por exemplo, futuramente `ObjetoAnatomia` poderia ter um método pra exibir camadas (pele, músculo, osso).

---

## 4. Como funciona, na prática

A cada frame da webcam, o `main` faz quatro coisas:

1. **Detecta os marcadores.** O `ArucoDetector` da OpenCV devolve os cantos de cada marcador (em pixels) e seus IDs.
2. **Estima a pose.** O `solvePnP` recebe os cantos detectados e os cantos correspondentes em 3D (a partir do tamanho físico do marcador, 8 cm) e devolve `rvec` (rotação) e `tvec` (translação).
3. **Pega o objeto.** `gerenciador.getObjeto(id)` retorna o `ObjetoAR*` correspondente, ou `nullptr` se o ID não estiver cadastrado.
4. **Renderiza.** O `render()` da subclasse:
   - calcula o bounding box do modelo pra centralizar e normalizar pra ~15 cm,
   - projeta os vértices 3D em 2D com `projectPoints`,
   - desenha as 3 arestas de cada triângulo com `cv::line`.

O efeito final é o wireframe acompanhando o marcador na imagem, em tempo real.

A escolha de wireframe (em vez de superfície preenchida) foi por simplicidade e estilo — fica com cara de "holograma" e não precisa de z-buffer nem iluminação.

---

## 5. Estrutura dos arquivos

```
ObjetoAR.h          ← classe base abstrata
ObjetoHistoria.h    ← intermediária + TorreEiffelAR + EstatuaAR
ObjetoAnatomia.h    ← intermediária + CranioAR
GerenciadorAR.h     ← map<int, ObjetoAR*>
main.cpp            ← loop de captura, detecção e render
generate_markers.cpp← utilitário pra gerar os PNGs dos marcadores
assets/             ← modelos .obj
markers/            ← marcadores ArUco em PNG
```

---

## 6. Limitações

- A matriz de câmera é aproximada (não calibramos com tabuleiro de xadrez). Pra precisão maior valeria rodar `cv::calibrateCamera`.
- O wireframe não tem oclusão — você vê o lado de trás do modelo "atravessando" o da frente. Resolver isso pediria z-buffer ou OpenGL.
- O catálogo de objetos é fixo no construtor do `GerenciadorAR`. Carregar de um JSON seria uma evolução natural.

---

## 7. Contribuições

- **Luiz Taiguara** — desenho da hierarquia de classes (`ObjetoAR`, `ObjetoHistoria`, `ObjetoAnatomia`), `GerenciadorAR`, redação do relatório.
- **Luiz Miguel** — loop principal (`main.cpp`), integração com ArUco e `solvePnP`, configuração do build (CMake + vcpkg), página GitHub Pages.
- **Kaique Bonfim** — carregamento dos `.obj` via Assimp, implementação dos `render()` das classes concretas, geração e teste dos marcadores.

As decisões de design e os testes em webcam foram feitos em conjunto.
