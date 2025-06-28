#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Model {
public:
    Model(const std::string& path, const std::string& texturePath);
    ~Model();

    void draw(Shader& shader);

private:
    // Dados do modelo
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;

    void loadModel(const std::string& path);
    void setupMesh();
    void loadTexture(const std::string& path);
};

#endif
