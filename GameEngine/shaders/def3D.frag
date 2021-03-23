#version 330 core

layout (location = 0) out vec4 color;

void main () 
{
    if (gl_FrontFacing) {
        color = vec4(1.0, 0.0, 1.0, 1.0);
    } else {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }
}