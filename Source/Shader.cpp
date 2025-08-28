#include <Shader.h>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <utils.h>

#define INFO_LOG_MAX_BUFFER 1024

void validateShaderCompilation(unsigned int shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[INFO_LOG_MAX_BUFFER];
        glGetShaderInfoLog(shader, INFO_LOG_MAX_BUFFER, NULL, infoLog);
        std::cerr << infoLog << std::endl;
    }
}

unsigned int compileShaderSource(const std::string& sourceStr, int type) {
    unsigned int shader = glCreateShader(type);
    const char* sourceBytes = sourceStr.c_str();
    glShaderSource(shader, 1, &sourceBytes, NULL);
    glCompileShader(shader);
    validateShaderCompilation(shader);
    return shader;
}

unsigned int compileShaderFile(const std::string& filepath, int type) {
    std::stringstream ss;
    std::ifstream file(asAbsolutePath(filepath));
    ss << file.rdbuf();
    return compileShaderSource(ss.str(), type);
}

unsigned int compileShaderSPIRV(const std::string& filepath, int type) {
    std::stringstream ss;
    std::ifstream file(asAbsolutePath(filepath));
    unsigned int shader = glCreateShader(type);
    const char* sourceBytes = ss.str().c_str();
    glShaderBinary(1, &shader, GL_SPIR_V_BINARY, sourceBytes, ss.str().length());

    glCompileShader(shader);
    validateShaderCompilation(shader);
    return shader;
}

Shader::Shader(const std::string& vertex, const std::string& fragment, ShaderInputType type) {
    unsigned int vs, fs;
    switch (type) {
        case AS_FILE: {
                vs = compileShaderFile(vertex, GL_VERTEX_SHADER);
                fs = compileShaderFile(fragment, GL_FRAGMENT_SHADER);
                break;
            }
        case AS_SPIRV_FILE: {
                vs = compileShaderSPIRV(vertex, GL_VERTEX_SHADER);
                fs = compileShaderSPIRV(fragment, GL_FRAGMENT_SHADER);
                break;
            }
        case AS_INLINE:
        default:
            vs = compileShaderSource(vertex, GL_VERTEX_SHADER);
            fs = compileShaderSource(fragment, GL_FRAGMENT_SHADER);
        }
    m_program = glCreateProgram();
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);
}

Shader::~Shader() {
    glDeleteProgram(m_program);
}

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::setFloat3(const std::string& uniform, float x, float y, float z) {
    glUniform3f(getUniformLocation(uniform), x, y, z);
}

void Shader::setTexture(const std::string& uniform, unsigned int index) {
    glUniform1i(getUniformLocation(uniform), index);
}

void Shader::setMat4(const std::string &uniform, const glm::mat4 &mat) {
    glUniformMatrix4fv(getUniformLocation(uniform), 1, false, glm::value_ptr(mat));
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_uniformCache.find(name) == m_uniformCache.end()) {
        int loc = glGetUniformLocation(m_program, name.c_str());
        if (loc < 0) std::cout << name << " is unavailable as a uniform!" << std::endl;
        m_uniformCache.emplace(name, loc);
        return loc;
    }
    return m_uniformCache.at(name);
}