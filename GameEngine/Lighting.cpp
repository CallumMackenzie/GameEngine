#include "Lighting.h"

void Light::sendLightsToShader(Shader* shader, Light* lightList, int nLights)
{
    for (int i = 0; i < nLights; i++)
    {
        std::string number = std::to_string(i);
        shader->setUniformVec3(("pointLights[" + number + "].position").c_str(), lightList[i].position);
        shader->setUniformVec3(("pointLights[" + number + "].ambient").c_str(), lightList[i].ambient);
        shader->setUniformVec3(("pointLights[" + number + "].diffuse").c_str(), lightList[i].diffuse * lightList[i].intensity);
        shader->setUniformVec3(("pointLights[" + number + "].specular").c_str(), lightList[i].specular);
        shader->setUniformFloat(("pointLights[" + number + "].constant").c_str(), 1.0f);
        shader->setUniformFloat(("pointLights[" + number + "].linear").c_str(), 0.09f);
        shader->setUniformFloat(("pointLights[" + number + "].quadratic").c_str(), 0.032f);
    }
}

void DirectionalLight::sendToShader(Shader* shader)
{
    shader->setUniformVec3("dirLight.direction", position);
    shader->setUniformVec3("dirLight.ambient", ambient);
    shader->setUniformVec3("dirLight.specular", specular);
    shader->setUniformVec3("dirLight.diffuse", diffuse);
}
