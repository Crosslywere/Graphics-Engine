#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
varying vec2 uv;
void main() {
    uv = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
}