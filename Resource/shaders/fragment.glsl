#version 330 core
uniform vec3 color;
uniform sampler2D wall;
varying vec2 uv;
void main() {
    vec3 wallFragment = texture2D(wall, uv).rgb;
    gl_FragColor = vec4((color + 1.0) * 0.5 * wallFragment, 1.0);
}
