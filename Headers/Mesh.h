#pragma once

#include <vector>
#include <glm/glm.hpp>

class Mesh {
public:
    class AttributeBuilder {
        friend class Mesh;
    public:
        AttributeBuilder() = default;
        AttributeBuilder(std::initializer_list<unsigned int> sizes);
        AttributeBuilder& addSize(unsigned int size);
    private:
        void build() const;
        std::vector<unsigned int> offsets;
        size_t stride{};
    };
public:
    using SizeDataPair = std::pair<int, const std::vector<float>>;
    Mesh(const unsigned int* const indices, int icount, const float* const vertices, int vcount, const AttributeBuilder& attribBuilder);
    Mesh(const unsigned int* const indices, int icount, const std::initializer_list<SizeDataPair>& dataPairs);
    Mesh(const std::vector<unsigned int>& indices, const std::initializer_list<SizeDataPair>& dataPairs);
    ~Mesh();
    void draw() const;
private:
    const unsigned int m_count;
    unsigned int m_VAO;
    unsigned int* m_buffers;
    int m_numBuffers;
};