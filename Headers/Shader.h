#pragma once

#include <unordered_map>
#include <string>

class Shader {
public:
    Shader(const std::string& vs, const std::string& fs);
    ~Shader();
    void use() const;
    void setFloat3(const std::string& uniform, float x, float y, float z);
private:
    int getUniformLocation(const std::string& name);
private:
    unsigned int m_program;
    std::unordered_map<std::string, int> m_uniformCache;
};