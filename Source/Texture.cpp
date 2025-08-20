#include <Texture.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const std::string& filepath, bool flipped, bool pixelated) {
    glGenTextures(1, &m_texture);
    stbi_set_flip_vertically_on_load(flipped);
    int ch;
    auto data = stbi_load(filepath.c_str(), &m_width, &m_height, &ch, 0);
    if (!data) {
        std::cerr << stbi_failure_reason() << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    std::cout << ch << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_texture);
}

unsigned int Texture::bind(unsigned int index) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    return index;
}
