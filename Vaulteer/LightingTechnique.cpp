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

void LightingTechnique::setDirectionalLight(const DirectionalLight& light) {
    using namespace Binder::deferred_frag::uniforms;
    /*shader.setUniform("directionalLight.light.color", light.color);
    shader.setUniform("directionalLight.light.ambientIntensity", light.ambientIntensity);
    shader.setUniform("directionalLight.light.diffuseIntensity", light.diffuseIntensity);

    shader.setUniform("directionalLight.direction", glm::normalize(light.direction));*/
}

void LightingTechnique::setPointLight(const PointLight& light, const int& index) {
    using namespace Binder::deferred_frag::uniforms;
    std::string indexStr = "pointLights[" + std::to_string(index) + "].";


    shader.setUniform({ "vec3", std::string(indexStr + "light.color").c_str(), 0, 12 }, 1, light.color);
    shader.setUniform({ "float", std::string(indexStr + "light.ambientIntensity").c_str(), 0, 4 }, light.ambientIntensity);
    shader.setUniform({ "float", std::string(indexStr + "light.diffuseIntensity").c_str(), 0, 4 }, light.diffuseIntensity);

    shader.setUniform({ "float", std::string(indexStr + "att.aConstant").c_str(), 0, 4 }, light.attenuation.constant);
    shader.setUniform({ "float", std::string(indexStr + "att.aLinear").c_str(), 0, 4 }, light.attenuation.linear);
    shader.setUniform({ "float", std::string(indexStr + "att.aQuadratic").c_str(), 0, 4 }, light.attenuation.quadratic);

    shader.setUniform({ "vec3", std::string(indexStr + "position").c_str(), 0, 12 }, 1, light.position);
}

void LightingTechnique::setSpotLight(const SpotLight& light) {
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
