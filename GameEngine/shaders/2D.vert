#version 330 core

layout (location = 0) in vec4 position;
// uniform mat4 projectionMatrix;
// uniform mat4 transMatrix;

void main () {
    // vec4 transformed = transMatrix * position;
    // transformed *= projectionMatrix;
    // gl_Position = transformed;
    gl_Position = position;
}