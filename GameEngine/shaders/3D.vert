#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 vertexUV;
layout (location = 2) in vec4 vertexRGB;
uniform mat4 modelViewMatrix;

out vec3 UV;
out vec4 colourRGB;

void main () {
    vec4 transformed = modelViewMatrix * position;
    if (transformed.w > 0.01) {
        transformed /= transformed.w;
        transformed.x *= -1.0;
        gl_Position = transformed;
    }

    UV = vertexUV;
    colourRGB = vertexRGB;
}