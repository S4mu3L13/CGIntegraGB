#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "ModelLoader.hpp"
#include "Trajectory.hpp"
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <filesystem>

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// --- Câmera ---
Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// --- Tempo ---
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- Estado do mouse ---
bool leftMousePressed = false;

// --- Controle de trajetória ---
bool followTrajectory = false;  // começa parado
std::vector<glm::vec3> trajectoryPoints = {
    {0,0,0}, {2,0,2}, {-2,0,2}
};
Trajectory traj(trajectoryPoints.data(), (int)trajectoryPoints.size());

// --- Transformações do objeto ---
glm::vec3 objPosition(0.0f);
glm::vec3 objRotation(0.0f);
glm::vec3 objScale(1.0f);

// --- Callbacks ---
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMousePressed = (action == GLFW_PRESS);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }
    float xoffset = float(xpos) - lastX;
    float yoffset = lastY - float(ypos);
    lastX = float(xpos);
    lastY = float(ypos);
    camera.processMouseMovement(xoffset, yoffset);
}

void updateTrajectory() {
    traj = Trajectory(trajectoryPoints.data(), (int)trajectoryPoints.size());
}

void processInput(GLFWwindow* window) {
    // Fecha
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Velocidade de câmera extra
    float speedMod = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 3.0f : 1.0f;
    float camDelta = deltaTime * speedMod;

    // WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboard(FORWARD, camDelta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboard(BACKWARD, camDelta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboard(LEFT, camDelta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboard(RIGHT, camDelta);

    // IJKL: vertical e lateral preciso
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) camera.Position += camera.Up * camDelta;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) camera.Position -= camera.Up * camDelta;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) camera.Position -= camera.Right * camDelta;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) camera.Position += camera.Right * camDelta;

    // Trajetória on/off com P
    static bool pLast = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pLast) {
        followTrajectory = !followTrajectory;
        std::cout << "Seguir trajetória: " << (followTrajectory ? "SIM" : "NÃO") << std::endl;
    }
    pLast = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;

    // Adiciona ponto na trajetória com clique+T
    static bool tLast = false;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !tLast) {
        if (leftMousePressed) {
            trajectoryPoints.push_back(objPosition);
            updateTrajectory();
            std::cout << "Ponto adicionado em: ("
                << objPosition.x << ", " << objPosition.y << ", " << objPosition.z << ")\n";
        }
        else {
            std::cout << "Pressione clique esquerdo + T para adicionar ponto\n";
        }
    }
    tLast = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

    // Escala do objeto [ e ]
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        objScale -= glm::vec3(0.5f) * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        objScale += glm::vec3(0.5f) * deltaTime;
    objScale = glm::max(objScale, glm::vec3(0.1f));

    // Rotação do objeto X, Y, Z
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        objRotation.x += glm::radians(90.0f) * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        objRotation.y += glm::radians(90.0f) * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        objRotation.z += glm::radians(90.0f) * deltaTime;

    // Movimento manual do objeto com setas (se não em trajetória)
    if (!followTrajectory) {
        const float objMoveSpeed = 2.0f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    objPosition += glm::vec3(0, 0, -1) * deltaTime * objMoveSpeed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  objPosition += glm::vec3(0, 0, 1) * deltaTime * objMoveSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  objPosition += glm::vec3(-1, 0, 0) * deltaTime * objMoveSpeed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) objPosition += glm::vec3(1, 0, 0) * deltaTime * objMoveSpeed;
    }
}

int main() {
    std::cout << "Diretório atual: " << std::filesystem::current_path() << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Visualizador 3D", NULL, NULL);
    if (!window) { std::cout << "Falha GLFW\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha GLAD\n"; return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    Model model("assets/models/artorias.obj", "assets/textures/textura1.png");

    while (!glfwWindowShouldClose(window)) {
        float now = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        processInput(window);
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        // Atualiza posição
        if (followTrajectory) {
            objPosition = traj.getPosition(now);
        }

        // Monta transformações
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, objPosition);
        modelMat = glm::rotate(modelMat, objRotation.x, glm::vec3(1, 0, 0));
        modelMat = glm::rotate(modelMat, objRotation.y, glm::vec3(0, 1, 0));
        modelMat = glm::rotate(modelMat, objRotation.z, glm::vec3(0, 0, 1));
        modelMat = glm::scale(modelMat, objScale);

        shader.setMat4("projection", proj);
        shader.setMat4("view", view);
        shader.setMat4("model", modelMat);
        shader.setVec3("lightDir", glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)));
        shader.setInt("texture_diffuse1", 0);

        model.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
