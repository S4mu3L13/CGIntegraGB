Autor: Samuel Pasquali  
Disciplina: Computação Gráfica  

-------------------------------------------
PRÉ-REQUISITOS
-------------------------------------------

Antes de compilar o projeto, certifique-se de que seu ambiente possui:

- Compilador C++ (Visual Studio ou g++, com suporte a C++17)
- OpenGL 3.3 ou superior
- GLFW (https://www.glfw.org/)
- GLAD (https://glad.dav1d.de/)
- GLM (https://github.com/g-truc/glm)
- stb_image.h (já incluso no projeto)

-------------------------------------------
COMO COMPILAR (Visual Studio)
-------------------------------------------

1. Crie um novo projeto no Visual Studio (Console App C++).
2. Adicione todos os arquivos `.cpp` e `.hpp` ao projeto.
3. Adicione os diretórios de `include/` no campo "Additional Include Directories".
4. Linke as bibliotecas: `opengl32.lib`, `glfw3.lib`, `glad.lib` (caso use estáticas).
5. Certifique-se de que os arquivos `.obj` e `.png` estão no caminho correto.

-------------------------------------------
COMO COMPILAR (g++ / Linux / WSL)
-------------------------------------------

g++ src/*.cpp -o Visualizador3D -lglfw -ldl -lGL -Iinclude

(Assumindo que os cabeçalhos glad/glm/stb estão em `include/`)

-------------------------------------------
COMO EXECUTAR
-------------------------------------------

1. Certifique-se de que os arquivos da pasta `assets` estão acessíveis no mesmo diretório do executável.
2. Rode o executável:

    ./Visualizador3D   (Linux/macOS)
    Visualizador3D.exe (Windows)

-------------------------------------------
CONTROLES
-------------------------------------------

- W, A, S, D: movimentar câmera
- Mouse: girar a visão
- Shift: acelerar movimento
- [, ]: escala do objeto
- X, Y, Z: rotação do objeto
- Tab: alterna objeto selecionado
- T + Clique: adiciona ponto de trajetória
- P: inicia ou pausa a animação

-------------------------------------------
OBSERVAÇÕES
-------------------------------------------

- O programa deve ser executado a partir da pasta raiz onde estão `assets/` e `shaders/`.
- Texturas são carregadas com `stb_image` (flip vertical já aplicado).
- Apenas modelos `.obj` com coordenadas de textura UV são suportados.