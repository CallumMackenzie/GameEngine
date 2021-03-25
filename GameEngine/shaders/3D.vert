#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 vertexUV;
uniform mat4 modelViewMatrix;

out vec3 UV;

void main () {
    vec4 transformed = modelViewMatrix * position;
    if (transformed.w > 0.001)
        transformed /= transformed.w;
    transformed.x *= -1.0;
    gl_Position = transformed;

    UV = vertexUV;
}