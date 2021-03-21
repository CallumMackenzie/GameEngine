#version 330 core

layout (location = 0) in vec4 position;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transMatrix;
uniform vec2 aspectRatio;

void main () {
    vec4 transformed = position * transMatrix * viewMatrix * projectionMatrix;
    transformed = vec4(transformed.x, transformed.y, 0.0, position.w); // Convert screen space to GL space
    gl_Position = transformed;
}