#version 330 core

layout (location = 0) out vec3 color;
uniform sampler2D textureSampler;
uniform float u_time;

in vec3 UV;
in vec4 colourRGB;

void main () 
{
    color = texture(textureSampler, vec2(UV.x / UV.z, 1 - (UV.y / UV.z))).rgb + colourRGB.rgb;
}