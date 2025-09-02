#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vFragPos;

void main() {
    vNormal = mat3(transpose(inverse(uModel))) * normalize(aNormal);
    vTexCoord = aTexCoord;
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}