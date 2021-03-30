#pragma once

#include "IngeniumConfig.h"
#include "3DData.h"

struct Light
{
	float intensity = 1;
	Vector3D colour;
	Vector3D position;
	Vector3D rotation;

	void sendToShader(Shader* shader);
};