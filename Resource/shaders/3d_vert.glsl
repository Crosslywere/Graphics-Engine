#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

varying vec2 vTexCoord;
varying vec3 vNormal;

void main() {
    vNormal = normalize(aNormal);
    vTexCoord = aTexCoord;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}