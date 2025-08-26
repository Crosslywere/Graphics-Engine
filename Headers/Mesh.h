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
    Mesh(const std::vector<float>& attributes, const std::vector<unsigned int>& indices, const AttributeBuilder& attributeBuilder);
    Mesh(const std::vector<SizeDataPair>& attributes, const std::vector<unsigned int>& indices);
    Mesh(Mesh&&) = default;
    Mesh(Mesh&) = default;
    Mesh() = default;
    ~Mesh();
    void draw() const;
private:
    const unsigned int m_count;
    unsigned int m_VAO;
    std::vector<unsigned int> m_Buffers;
};