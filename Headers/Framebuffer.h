#pragma once

// #include <>

class Framebuffer {
public:
    static void clear();
    static void setClearColor(float r, float g, float b, float a = 1);
};