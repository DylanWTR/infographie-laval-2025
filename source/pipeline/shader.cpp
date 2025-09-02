#include "pipeline/shader.hpp"
#include "glad.hpp"

// STD Include //
#include <fstream>
#include <sstream>

// GLFW Include //
#include <GLFW/glfw3.h>

#include "exception/shader-exception.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    const std::string vertexShaderSource = readShaderFile(vertexPath);
    const std::string fragmentShaderSource = readShaderFile(fragmentPath);

    if (vertexShaderSource.empty() || fragmentShaderSource.empty())
        throw ShaderException("Failed to load shader source files");

    // Compile Vertex Shader //
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSourceCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Compile Fragment Shader //
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSourceCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Link Shaders //
    _id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);
    checkCompileErrors(_id, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::readShaderFile(const std::string &filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (!file.is_open())
        throw ShaderException("Failed to open file: " + filePath);
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::checkCompileErrors(const unsigned int shader, const std::string &type) const {
    char infoLog[512];
    int success;

    if (type == "PROGRAM") {
        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            _logger.error("Shader ({}): {}", type, infoLog);
        }
        return;
    }
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        _logger.warn("Shader ({}): {}", type, infoLog);
    }
}

void Shader::use() const {
    glUseProgram(_id);
}

unsigned int Shader::getId() const {
    return _id;
}

void Shader::setBool(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, const float x, const float y) const {
    glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
