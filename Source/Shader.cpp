#include <Shader.h>
#include <glad/glad.h>
#include <iostream>

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

unsigned int compileShaderSource(const std::string& source, int type) {
    unsigned int shader = glCreateShader(type);
    const char* sourceData = source.c_str();
    glShaderSource(shader, 1, &sourceData, NULL);
    glCompileShader(shader);
    validateShaderCompilation(shader);
    return shader;
}

Shader::Shader(const std::string& vertex, const std::string& fragment, ShaderInputType type) {
    unsigned int vs, fs;
    switch (type) {
        case AS_FILE: {
                std::cerr << "File loading has not been implemented!" << std::endl;
                exit(EXIT_FAILURE);
                break;
            }
        case AS_SPIRV_FILE: {
                std::cerr << "SPIR-V has not been implemented!" << std::endl;
                exit(EXIT_FAILURE);
                break;
            }
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

int Shader::getUniformLocation(const std::string& name) {
    if (m_uniformCache.find(name) == m_uniformCache.end()) {
        int loc = glGetUniformLocation(m_program, name.c_str());
        if (loc < 0) std::cout << name << " is unavailable as a uniform!" << std::endl;
        m_uniformCache.emplace(name, loc);
        return loc;
    }
    return m_uniformCache.at(name);
}