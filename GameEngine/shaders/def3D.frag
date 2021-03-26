#version 330 core

layout (location = 0) out vec3 color;
uniform sampler2D textureSampler;
uniform bool hasTexture;
uniform float time;

in vec3 UV;

void main () 
{
    if (hasTexture) {
        color = texture(textureSampler, UV.xy).rgb;
    } else {
        vec2 st = vec2(gl_FragCoord.x / 1600.0, gl_FragCoord.y / 900.0);
        color = UV * vec3(st, (st.x + st.y) / 2);
    }
}