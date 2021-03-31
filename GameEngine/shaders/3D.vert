#version 330 core

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec3 vertexUV;
layout (location = 2) in vec4 vertexRGB;
layout (location = 3) in vec4 vertexNormal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 invModel;

out vec3 UV;
out vec4 tint;
out vec3 normal;
out vec3 fragPos;

void main () {
    vec4 transformed = projection * view * model * vertexPosition;

    if (transformed.w > 0) {
        float w = transformed.w;
        transformed /= transformed.w;
        transformed.x *= -1.0;
        gl_Position = transformed;
        UV = vec3(vertexUV.x / w, vertexUV.y / w, 1.0 / w);
        tint = vertexRGB.rgba;
        normal =  mat3(transpose(invModel)) * vertexNormal.xyz;
        fragPos = vec3(model * vertexPosition);
    }
}