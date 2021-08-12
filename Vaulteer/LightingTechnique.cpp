#include "LightingTechnique.h"



LightingTechnique::LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode)
    : Technique(pathGLSLVertexCode, pathGLSLFragmentCode)
{
}

void LightingTechnique::init() {
    using namespace Binder::deferred_frag::uniforms;
    shader.setUniform(gPosition, 0);
    shader.setUniform(gNormal, 1);
    shader.setUniform(gColor, 2);
}

void LightingTechnique::setDirectionalLight(const GLSLDirectionalLight& light) {
    using namespace Binder::deferred_frag::uniforms;
    /*shader.setUniform("directionalLight.light.color", light.color);
    shader.setUniform("directionalLight.light.ambientIntensity", light.ambientIntensity);
    shader.setUniform("directionalLight.light.diffuseIntensity", light.diffuseIntensity);

    shader.setUniform("directionalLight.direction", glm::normalize(light.direction));*/
}

void LightingTechnique::setPointLight(const GLSLPointLight& light, const int& index) {
    using namespace Binder::deferred_frag::uniforms;

    shader.setUniform(pointLights[index].light.color, 1, light.color);
    shader.setUniform(pointLights[index].light.ambientIntensity, light.ambientIntensity);
    shader.setUniform(pointLights[index].light.diffuseIntensity, light.diffuseIntensity);

    shader.setUniform(pointLights[index].att.aConstant, light.attenuation.constant);
    shader.setUniform(pointLights[index].att.aLinear, light.attenuation.linear);
    shader.setUniform(pointLights[index].att.aQuadratic, light.attenuation.quadratic);

    shader.setUniform(pointLights[index].position, 1, light.position);
}

void LightingTechnique::setSpotLight(const GLSLSpotLight& light) {
    using namespace Binder::deferred_frag::uniforms;
    /*shader.setVector("spotLight.base.light.color", light.color);
    shader.setFloat("spotLight.base.light.ambientIntensity", light.ambientIntensity);
    shader.setFloat("spotLight.base.light.diffuseIntensity", light.diffuseIntensity);

    shader.setFloat("spotLight.base.att.aConstant", light.attenuation.constant);
    shader.setFloat("spotLight.base.att.aLinear", light.attenuation.linear);
    shader.setFloat("spotLight.base.att.aQuadratic", light.attenuation.quadratic);

    shader.setVector("spotLight.base.position", light.position);

    shader.setVector("spotLight.direction", light.direction);
    shader.setFloat("spotLight.cutoff", light.cutoff); */
}

void LightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader.setUniform(Binder::deferred_frag::uniforms::worldCameraPos, 1, cameraPos);
}

void LightingTechnique::setMaterialSpecularIntensity(const float intensity) {
    shader.setUniform(Binder::deferred_frag::uniforms::materialSpecularIntensity, intensity);
}

void LightingTechnique::setMaterialShininess(const float shininess) {
    shader.setUniform(Binder::deferred_frag::uniforms::materialShininess, shininess);
}
