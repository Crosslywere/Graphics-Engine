#include <Mesh.h>
#include <glad/glad.h>
#include <cstdarg>
#include <iostream>

Mesh::AttributeBuilder::AttributeBuilder(std::initializer_list<unsigned int> sizes) {
    for (auto b = sizes.begin(); b != sizes.end(); b++) {
        offsets.push_back(*b);
        stride += *b;
    }
}

Mesh::AttributeBuilder& Mesh::AttributeBuilder::addSize(unsigned int size) {
    offsets.push_back(size);
    stride += size;
    return *this;
}

void Mesh::AttributeBuilder::build() const {
    size_t offset{};
    for (int i = 0; i < offsets.size(); i++) {
        glVertexAttribPointer(i, offsets.at(i), GL_FLOAT, false, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += offsets.at(i);
    }
}

Mesh::Mesh(const std::vector<float> &attributes, const std::vector<unsigned int> &indices, const AttributeBuilder &attributeBuilder) 
        : m_count(indices.size()) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(float), attributes.data(), GL_STATIC_DRAW);
    m_Buffers.push_back(buffer);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    m_Buffers.push_back(buffer);
    attributeBuilder.build();
}

Mesh::Mesh(const std::vector<SizeDataPair>& attributes, const std::vector<unsigned int>& indices)
        : m_count(indices.size()) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    unsigned int buffer;
    for (int i = 0; i < attributes.size(); i++) {
        auto [size, data] = attributes.at(i);
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(i, size, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(i);
        m_Buffers.push_back(buffer);
    }
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    m_Buffers.push_back(buffer);
}

Mesh::~Mesh() {
    glDeleteBuffers(m_Buffers.size(), m_Buffers.data());
    glDeleteVertexArrays(1, &m_VAO);
    m_Buffers.clear();
}

void Mesh::draw() const {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, NULL);
}