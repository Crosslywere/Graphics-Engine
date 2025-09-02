#pragma once

#include <vector>

class Framebuffer {
    friend class Window;
public:
    static void unbind();
    static void clear();
    static void setClearColor(float r, float g, float b, float a = 1);
    static void enableDepthTest(bool enable = true);
    static void enableCullFace(bool enable = true);
public:
    Framebuffer(int width, int height);
    ~Framebuffer();
    void bind() const;
    void drawToScreen() const;
    unsigned int bindColorTexture(unsigned int index = 0) const;
    unsigned int bindDepthTexture(unsigned int index = 1) const;
private:
    static void init();
    static void deinit();
    unsigned int m_Framebuffer;
    std::vector<unsigned int> m_Textures;
};