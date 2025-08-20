#pragma once

#include <string>

class Texture {
public:
    Texture(const std::string& filepath, bool flipped = false, bool pixelated = false);
    ~Texture();
    unsigned int bind(unsigned int index = 0) const;
private:
    unsigned int m_texture;
    int m_width, m_height;
};