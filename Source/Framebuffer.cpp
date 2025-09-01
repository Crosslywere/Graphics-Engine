#include <Framebuffer.h>
#include <glad/glad.h>
#include <Shader.h>
#include <Mesh.h>

static Shader* s_Shader = nullptr;
static Mesh* s_Mesh = nullptr;
static bool s_Init = false;
static bool s_Depth = false;

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | s_Depth ? GL_DEPTH_BUFFER_BIT : 0 | GL_STENCIL_BUFFER_BIT);
}

void Framebuffer::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Framebuffer::enableDepthTest(bool enable) {
    if (enable) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
    s_Depth = enable;
}

Framebuffer::Framebuffer(int width, int height) {
    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

    glGenRenderbuffers(1, &m_Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
    glDeleteRenderbuffers(1, &m_Renderbuffer);
    glDeleteTextures(1, &m_Texture);
    glDeleteFramebuffers(1, &m_Framebuffer);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void Framebuffer::drawToScreen() const {
    unbind();
    enableDepthTest(false);
    s_Shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    s_Mesh->draw();
}

void Framebuffer::init() {
    if (s_Init) return;
    s_Shader = new Shader(
        R"(#version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 vTexCoord;
        void main() {
        vTexCoord = aTexCoord;
        gl_Position = vec4(aPos, 0.0, 1.0);
        })",
        R"(#version 330 core
        out vec4 oFragColor;
        in vec2 vTexCoord;
        uniform sampler2D uTexture;
        void main() {
        oFragColor = texture2D(uTexture, vTexCoord);
        })"
    );
    s_Mesh = new Mesh(
        {-1, 1, 0, 1, -1, -1, 0, 0, 1, -1, 1, 0, 1, 1, 1, 1},
        {0, 1, 2, 2, 3, 0},
        {2, 2}
    );
    s_Init = true;
}

void Framebuffer::deinit() {
    if (!s_Init) return;
    delete s_Shader;
    delete s_Mesh;
    s_Init = false;
}
