#include "LightingTechnique.h"


void LightingTechnique::init() {
    using namespace Binder::deferred_frag::uniforms;

    setUniform(gPosition, 0);
    setUniform(gNormal, 1);
    setUniform(gColor, 2);
    setUniform(shadowMap_0, 3);
    setUniform(shadowMap_1, 4);
    setUniform(shadowMap_2, 5);
    setUniform(shadowCubeMap_0, 6);
    setUniform(shadowCubeMap_1, 7);
    setUniform(shadowCubeMap_2, 8);
    setUniform(shadowCubeMap_3, 9);
}

void LightingTechnique::setDirectionalLight(const GLSLDirectionalLight& light) {
    using namespace Binder::deferred_frag::uniforms;
    setUniform(directionalLight.light.color, 1,light.color);
    setUniform(directionalLight.light.ambientIntensity, light.ambientIntensity);
    setUniform(directionalLight.light.diffuseIntensity, light.diffuseIntensity);

    setUniform(directionalLight.direction, 1, glm::normalize(light.direction));
}

void LightingTechnique::setPointLight(const GLSLPointLight& light, const int& index) {
    using namespace Binder::deferred_frag::uniforms;

    setUniform(pointLights[index].light.color, 1, light.color);
    setUniform(pointLights[index].light.ambientIntensity, light.ambientIntensity);
    setUniform(pointLights[index].light.diffuseIntensity, light.diffuseIntensity);

    setUniform(pointLights[index].att.aConstant, light.attenuation.constant);
    setUniform(pointLights[index].att.aLinear, light.attenuation.linear);
    setUniform(pointLights[index].att.aQuadratic, light.attenuation.quadratic);

    setUniform(pointLights[index].radius, light.radius);

    setUniform(pointLights[index].position, 1, light.position);
}

void LightingTechnique::setSpotLight(const GLSLSpotLight& light) {
    using namespace Binder::deferred_frag::uniforms;
    /*setVector("spotLight.base.light.color", light.color);
    setFloat("spotLight.base.light.ambientIntensity", light.ambientIntensity);
    setFloat("spotLight.base.light.diffuseIntensity", light.diffuseIntensity);

    setFloat("spotLight.base.att.aConstant", light.attenuation.constant);
    setFloat("spotLight.base.att.aLinear", light.attenuation.linear);
    setFloat("spotLight.base.att.aQuadratic", light.attenuation.quadratic);

    setVector("spotLight.base.position", light.position);

    setVector("spotLight.direction", light.direction);
    setFloat("spotLight.cutoff", light.cutoff); */
}

void LightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    setUniform(Binder::deferred_frag::uniforms::worldCameraPos, 1, cameraPos);
}

void LightingTechnique::setCameraViewMat(const glm::mat4& viewMat) {
    setUniform(Binder::deferred_frag::uniforms::cameraViewMat, 1, GL_FALSE, viewMat);
}

void LightingTechnique::setMaterialSpecularIntensity(const float intensity) {
    setUniform(Binder::deferred_frag::uniforms::materialSpecularIntensity, intensity);
}

void LightingTechnique::setMaterialShininess(const float shininess) {
    setUniform(Binder::deferred_frag::uniforms::materialShininess, shininess);
}

void LightingTechnique::setShadowMapData(ShadowRenderer& shadowRenderer) {
    using namespace Binder::deferred_frag;
    for (unsigned int i = 0; i < shadowRenderer.numCascades; i++) {
        setUniform(uniforms::lightSpaceMatrices[i], 1, GL_FALSE, shadowRenderer.getCascade(i).getLightSpaceMatrix());
        setUniform(uniforms::cascadeFarPlanes[i], shadowRenderer.getCascade(i).zFar);
    }

    for (unsigned int i = 0; i < shadowRenderer.numPointBuffers; i++) {
        setUniform(uniforms::cubeMapFarPlanes[i], shadowRenderer.getPointBuffer(i).farPlane);
    }
}

void LightingTechnique::setFogColor(const glm::vec3& color) {
    setUniform(Binder::deferred_frag::uniforms::fogColor, 1, color);
}