#include "Lighting.h"

void Light::sendToShader(Shader* shader)
{
	shader->setUniformVec4(SHADER_LIGHTING_COLOUR, colour);
	shader->setUniformVec3(SHADER_LIGHTING_ROTATION, rotation);
	shader->setUniformVec3(SHADER_LIGHTING_POSITION, position);
	shader->setUniformFloat(SHADER_LIGHTING_INTENSITY, intensity);
}
