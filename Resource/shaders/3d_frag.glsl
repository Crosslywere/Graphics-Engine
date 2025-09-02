#version 330 core

layout (location = 0) out vec4 oFragColor;
varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vFragPos;
uniform sampler2D uTexture;
uniform vec3 uLightPos;

void main() {
    vec3 diffuseFragment = vec3(texture2D(uTexture, vTexCoord));
    float ambient = 0.1;
    vec3 lightDir = normalize(uLightPos - vFragPos);
    float diffuse = max(dot(vNormal, lightDir), 0.0);
    oFragColor = vec4(diffuseFragment * min(diffuse + ambient, 1.0), 1.0);
}