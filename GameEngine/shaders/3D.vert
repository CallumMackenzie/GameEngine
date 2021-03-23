#version 330 core

layout (location = 0) in vec4 position;
uniform mat4 modelViewMatrix;
uniform vec2 aspectRatio;

void main () {
    vec4 transformed = modelViewMatrix * position;
    transformed /= transformed.w;
    transformed.x *= -1.0;
    // if (transformed.x >= -1.0 && transformed.x <= 1.0 && transformed.y >= -1.0 && transformed.y <= 1.0) {
        gl_Position = transformed;
    // }
}