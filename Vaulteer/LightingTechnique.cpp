#include "LightingTechnique.h"



LightingTechnique::LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode)
    : Technique(pathGLSLVertexCode, pathGLSLFragmentCode)
{
}

void LightingTechnique::setModel(const glm::mat4& model) {
    shader.setMatrix("model", model);
    setNormal(model);
}

void LightingTechnique::setView(const glm::mat4& view) {
    shader.setMatrix("view", view);
}

void LightingTechnique::setProjection(const glm::mat4& projection) {
    shader.setMatrix("projection", projection);
}

void LightingTechnique::setNormal(const glm::mat4& model) {
    shader.setMatrix("normal", glm::transpose(glm::inverse(model)));
}

void LightingTechnique::setTextureUnit(GLuint textureUnit) {
    shader.setInt("u_texture", textureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight& light) {
    shader.setVector("directionalLight.light.color", light.color);
    shader.setFloat("directionalLight.light.ambientIntensity", light.ambientIntensity);
    shader.setFloat("directionalLight.light.diffuseIntensity", light.diffuseIntensity);

    shader.setVector("directionalLight.direction", glm::normalize(light.direction));
}

void LightingTechnique::setPointLight(const PointLight& light) {
    shader.setVector("pointLight.light.color", light.color);
    shader.setFloat("pointLight.light.ambientIntensity", light.ambientIntensity);
    shader.setFloat("pointLight.light.diffuseIntensity", light.diffuseIntensity);

    shader.setFloat("pointLight.att.aConstant", light.attenuation.constant);
    shader.setFloat("pointLight.att.aLinear", light.attenuation.linear);
    shader.setFloat("pointLight.att.aQuadratic", light.attenuation.quadratic);

    shader.setVector("pointLight.position", light.position);
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
