#include "LightingTechnique.h"



LightingTechnique::LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode)
    : Technique(pathGLSLVertexCode, pathGLSLFragmentCode)
{
}

void LightingTechnique::setModel(const glm::mat4& model) {
    shader.setMatrix("model", model);
}

void LightingTechnique::setView(const glm::mat4& view) {
    shader.setMatrix("view", view);
}

void LightingTechnique::setProjection(const glm::mat4& projection) {
    shader.setMatrix("projection", projection);
}

void LightingTechnique::setTextureUnit(GLuint textureUnit) {
    shader.setInt("u_texture", textureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight& light) {
    shader.setFloat("directionalLight.ambientIntensity", light.ambientIntensity);
    shader.setVector("directionalLight.color", light.color);
    shader.setFloat("directionalLight.diffuseIntensity", light.diffuseIntensity);
    shader.setVector("directionalLight.direction", glm::normalize(light.direction));
}

void LightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader.setVector("worldCameraPos", cameraPos);
}

void LightingTechnique::setMaterialSpecularIntensity(const float intensity) {
    shader.setFloat("materialSpecularIntensity", intensity);
}

void LightingTechnique::setMaterialSpecularPower(const float power) {
    shader.setFloat("specularPower", power);
}
