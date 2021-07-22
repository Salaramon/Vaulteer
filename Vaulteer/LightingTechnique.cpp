#include "LightingTechnique.h"



LightingTechnique::LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode)
    : Technique(pathGLSLVertexCode, pathGLSLFragmentCode)
{
}

void LightingTechnique::init() {
    shader.setInt("gPosition", 0);
    shader.setInt("gNormal", 1);
    shader.setInt("gColor", 2);
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

void LightingTechnique::setSpotLight(const SpotLight& light) {
    shader.setVector("spotLight.base.light.color", light.color);
    shader.setFloat("spotLight.base.light.ambientIntensity", light.ambientIntensity);
    shader.setFloat("spotLight.base.light.diffuseIntensity", light.diffuseIntensity);

    shader.setFloat("spotLight.base.att.aConstant", light.attenuation.constant);
    shader.setFloat("spotLight.base.att.aLinear", light.attenuation.linear);
    shader.setFloat("spotLight.base.att.aQuadratic", light.attenuation.quadratic);

    shader.setVector("spotLight.base.position", light.position);

    shader.setVector("spotLight.direction", light.direction);
    shader.setFloat("spotLight.cutoff", light.cutoff);
}

void LightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader.setVector("worldCameraPos", cameraPos);
}

void LightingTechnique::setMaterialSpecularIntensity(const float intensity) {
    shader.setFloat("materialSpecularIntensity", intensity);
}

void LightingTechnique::setMaterialShininess(const float shininess) {
    shader.setFloat("materialShininess", shininess);
}
