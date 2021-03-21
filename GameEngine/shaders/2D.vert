#version 330 core

layout (location = 0) in vec4 position;
uniform mat4 projectionMatrix;
uniform mat4 transMatrix;
uniform vec2 aspectRatio;

void main () {
    // vec4 transformed = transMatrix * position;
    // transformed *= projectionMatrix;
    // gl_Position = transformed;
    vec4 transformed = vec4(
        ((position.x) / (aspectRatio.x * 100) * 2) - 1,
        - ((position.y) / (aspectRatio.y * 100) * 2) + 1,
        position.z, position.w); // Convert screen space to GL space
    gl_Position = transformed;
}