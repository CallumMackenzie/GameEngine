#pragma once

#include "IngeniumConfig.h"
#include "3DData.h"
#include "LinkedList.h"

struct Light;
struct DirectionalLight;

struct Light
{
	float intensity = 1;
	Vector3D ambient = { 0.1, 0.1, 0.1 };
	Vector3D diffuse =  { 0.5, 0.5, 0.5 };
	Vector3D specular = { 0.3, 0.3, 0.3 };
	Vector3D position;

	static void sendLightsToShader(Shader* shader, Light* lightList, int nLights);
};

struct DirectionalLight : Light {
	void sendToShader(Shader* shader);
};