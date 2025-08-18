#include <Shader.h>
#include <glad/glad.h>
#include <iostream>

unsigned int compileShaderSource(const std::string& source, int type) {
    unsigned int shader = glCreateShader(type);
    const char* rawSource = source.c_str();
    glShaderSource(shader, 1, &rawSource, NULL);
    glCompileShader(shader);
    return shader;
}

Shader::Shader(const std::string& vs, const std::string& fs) {
    m_program = glCreateProgram();
    auto vertex = compileShaderSource(vs, GL_VERTEX_SHADER);
    auto fragment = compileShaderSource(fs, GL_FRAGMENT_SHADER);
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
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

int Shader::getUniformLocation(const std::string& name) {
    if (m_uniformCache.find(name) == m_uniformCache.end()) {
        int loc = glGetUniformLocation(m_program, name.c_str());
        if (loc < 0) std::cout << name << " is unavailable as a uniform!" << std::endl;
        m_uniformCache.emplace(name, loc);
        return loc;
    }
    return m_uniformCache.at(name);
}