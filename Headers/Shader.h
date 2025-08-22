#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

enum ShaderInputType {
    AS_INLINE = 0,
    AS_FILE,
    AS_SPIRV_FILE,
};

class Shader {
public:
    Shader(const std::string& vertex, const std::string& fragment, ShaderInputType type =  AS_INLINE);
    ~Shader();
    void use() const;
    void setFloat3(const std::string& uniform, float x, float y, float z);
    void setTexture(const std::string& uniform, unsigned int index);
    void setMat4(const std::string& uniform, const glm::mat4& mat);
private:
    int getUniformLocation(const std::string& name);
private:
    unsigned int m_program;
    std::unordered_map<std::string, int> m_uniformCache;
};