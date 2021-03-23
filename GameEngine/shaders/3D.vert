#version 330 core

layout (location = 0) in vec4 position;
uniform mat4 renderMatrix;
uniform vec2 aspectRatio;

void main () {
    vec4 transformed = renderMatrix * position;
    transformed /= transformed.w;
    transformed.x *= -1.0;
    gl_Position = transformed;
}