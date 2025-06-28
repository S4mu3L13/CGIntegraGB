#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setMat4(const std::string& name, const glm::mat4& mat) const;

    void setInt(const std::string& name, int value) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
};

#endif
