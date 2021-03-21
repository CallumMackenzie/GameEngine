#version 330 core

layout (location = 0) in vec4 position;
uniform mat4 renderMatrix;
uniform vec2 aspectRatio;

void main () {
    vec4 transformed = position * renderMatrix;
    transformed /= transformed.w; 
    transformed.x *= -1.0;
    transformed.y *= -1.0;
    transformed += vec4(1.0, 1.0, 0.0, 0.0);
    transformed.x *= 0.5 * (aspectRatio.x * 100);
    transformed.y *= 0.5 * (aspectRatio.y * 100);
    transformed = vec4(
        ((transformed.x) / (aspectRatio.x * 100) * 2) - 1,
        - ((transformed.y) / (aspectRatio.y * 100) * 2) + 1,
        transformed.z, transformed.w); // Convert screen space to GL space
    gl_Position = transformed;
}