#version 330 core

layout (location = 0) out vec4 oFragColor;
varying vec2 vTexCoord;
varying vec3 vNormal;
uniform sampler2D uTexture;

void main() {
    vec3 t = texture2D(uTexture, vTexCoord).rgb;
    oFragColor = vec4(abs(vNormal) * t, 1.0);
}