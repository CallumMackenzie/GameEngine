#version 330 core

layout (location = 0) out vec4 color;
uniform sampler2D textureSampler;
uniform float u_time;

uniform vec4 lightColour;
uniform vec3 lightRotation;
uniform vec3 lightPosition;
uniform float lightIntensity;

in vec3 UV;
in vec4 tint;
in vec3 normal;
in vec3 fragPos;

void main () 
{
    vec3 norm = normalize(normal);

    vec4 ambient = lightIntensity * lightColour;

    vec3 lightDir = normalize(lightRotation);
    vec3 lightPos = normalize(lightDir - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColour;
    vec4 texRGB = texture(textureSampler, vec2(UV.x / UV.z, 1 - (UV.y / UV.z))).rgba;
    vec4 result = (diffuse + ambient) * (texRGB + tint);
    result.w = 1.0;
    color = result;
}