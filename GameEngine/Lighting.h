#pragma once

#include "IngeniumConfig.h"
#include "3DData.h"
#include "LinkedList.h"

struct Light;
struct DirectionalLight;

struct Light
{
	float intensity = 1;
	Vec3 ambient = { 0.1, 0.1, 0.1 };
	Vec3 diffuse =  { 0.5, 0.5, 0.5 };
	Vec3 specular = { 0.3, 0.3, 0.3 };
	Vec3 position;
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	static void sendLightsToShader(Shader* shader, Light* lightList, int nLights);
};

struct DirectionalLight : Light {
	void sendToShader(Shader* shader);
};