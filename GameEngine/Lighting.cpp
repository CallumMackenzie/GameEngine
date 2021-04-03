#include "Lighting.h"

void Light::sendLightsToShader(Shader* shader, Light* lightList, int nLights)
{
    for (int i = 0; i < nLights; i++)
    {
        std::string number = std::to_string(i);
        shader->setUniformVec3(("pointLights[" + number + "].position").c_str(), lightList[i].position);
        shader->setUniformVec3(("pointLights[" + number + "].ambient").c_str(), lightList[i].ambient);
        shader->setUniformVec3(("pointLights[" + number + "].diffuse").c_str(), lightList[i].diffuse * lightList[i].intensity);
        shader->setUniformVec3(("pointLights[" + number + "].specular").c_str(), lightList[i].specular * lightList[i].intensity);
        shader->setUniformFloat(("pointLights[" + number + "].constant").c_str(), lightList[i].constant);
        shader->setUniformFloat(("pointLights[" + number + "].linear").c_str(), lightList[i].linear);
        shader->setUniformFloat(("pointLights[" + number + "].quadratic").c_str(), lightList[i].quadratic);
    }
}

void DirectionalLight::sendToShader(Shader* shader)
{
    shader->setUniformVec3("dirLight.direction", position);
    shader->setUniformVec3("dirLight.ambient", ambient);
    shader->setUniformVec3("dirLight.specular", specular * intensity);
    shader->setUniformVec3("dirLight.diffuse", diffuse * intensity);
}
