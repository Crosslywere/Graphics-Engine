#version 330 core
layout (location = 0) out vec4 oPixelColor;
uniform vec3 uColor;
void main() {
    oPixelColor = vec4(uColor, 1.0);
}