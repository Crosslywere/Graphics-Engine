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

Mesh::Mesh(const unsigned int* const indices, int icount, const float* const vertices, int vcount, const AttributeBuilder& attribBuilder)
        : m_count(icount) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    m_buffers = new unsigned int[2];
    glGenBuffers(m_numBuffers = 2, m_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    attribBuilder.build();
}

Mesh::Mesh(const unsigned int *const indices, int icount, const std::initializer_list<SizeDataPair> &dataPairs)
        : m_count(icount), m_numBuffers(dataPairs.size() + 1) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    m_buffers = new unsigned int[m_numBuffers];
    glGenBuffers(dataPairs.size() + 1, m_buffers);
    for (int i = 0; i < dataPairs.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, dataPairs.begin()[i].second.size() * sizeof(float), dataPairs.begin()[i].second.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(i, dataPairs.begin()[i].first, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[dataPairs.size()]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

Mesh::Mesh(const std::vector<unsigned int>& indices, const std::initializer_list<SizeDataPair>& dataPairs)
        : m_count(indices.size()), m_numBuffers(dataPairs.size() + 1) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    m_buffers = new unsigned int[m_numBuffers];
    glGenBuffers(dataPairs.size() + 1, m_buffers);
    for (int i = 0; i < dataPairs.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, dataPairs.begin()[i].second.size() * sizeof(float), dataPairs.begin()[i].second.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(i, dataPairs.begin()[i].first, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[dataPairs.size()]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

Mesh::~Mesh() {
    glDeleteBuffers(m_numBuffers, m_buffers);
    glDeleteVertexArrays(1, &m_VAO);
    delete[] m_buffers;
}

void Mesh::draw() const {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, NULL);
}