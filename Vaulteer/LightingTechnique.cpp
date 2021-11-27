#include "LightingTechnique.h"


void LightingTechnique::init() {
    int texId = 0;

    setUniform(fragUnis::gPosition, texId++);
    setUniform(fragUnis::gNormal, texId++);
    setUniform(fragUnis::gColor, texId++);
    setUniform(fragUnis::shadowMap_0, texId++);
    setUniform(fragUnis::shadowMap_1, texId++);
    setUniform(fragUnis::shadowMap_2, texId++);
    setUniform(fragUnis::shadowCubeMap_0, texId++);
    setUniform(fragUnis::shadowCubeMap_1, texId++);
    setUniform(fragUnis::shadowCubeMap_2, texId++);
    setUniform(fragUnis::shadowCubeMap_3, texId++);
    setUniform(fragUnis::shadowSpotMap_0, texId++);
}

void LightingTechnique::setDirectionalLight(const GLSLDirectionalLight& light) {
    setUniform(fragUnis::directionalLight.light.color, 1,light.color);
    setUniform(fragUnis::directionalLight.light.ambientIntensity, light.ambientIntensity);
    setUniform(fragUnis::directionalLight.light.diffuseIntensity, light.diffuseIntensity);

    setUniform(fragUnis::directionalLight.direction, 1, glm::normalize(light.direction));
}

void LightingTechnique::setPointLight(const GLSLPointLight& light, const int index) {
    setUniform(fragUnis::pointLights[index].light.color, 1, light.color);
    setUniform(fragUnis::pointLights[index].light.ambientIntensity, light.ambientIntensity);
    setUniform(fragUnis::pointLights[index].light.diffuseIntensity, light.diffuseIntensity);

    setUniform(fragUnis::pointLights[index].att.aConstant, light.attenuation.constant);
    setUniform(fragUnis::pointLights[index].att.aLinear, light.attenuation.linear);
    setUniform(fragUnis::pointLights[index].att.aQuadratic, light.attenuation.quadratic);

    setUniform(fragUnis::pointLights[index].radius, GLSLPointLight::calculateRadius(light));

    setUniform(fragUnis::pointLights[index].position, 1, light.position);
}

void LightingTechnique::setSpotLight(const GLSLSpotLight& light, const int index) {
    setUniform(fragUnis::spotLights[index].light.color, 1, light.color);
    setUniform(fragUnis::spotLights[index].light.ambientIntensity, light.ambientIntensity);
    setUniform(fragUnis::spotLights[index].light.diffuseIntensity, light.diffuseIntensity);

    setUniform(fragUnis::spotLights[index].att.aConstant, light.attenuation.constant);
    setUniform(fragUnis::spotLights[index].att.aLinear, light.attenuation.linear);
    setUniform(fragUnis::spotLights[index].att.aQuadratic, light.attenuation.quadratic);

    setUniform(fragUnis::spotLights[index].position, 1, light.position);

    setUniform(fragUnis::spotLights[index].direction, 1, light.direction);
    setUniform(fragUnis::spotLights[index].angle, light.cutoffAngle);

    setUniform(fragUnis::spotLights[index].radius, GLSLSpotLight::calculateRadius(light));
    setUniform(fragUnis::spotLightSpaceMatrices[index], 1, GL_FALSE, GLSLSpotLight::getLightSpaceMatrix(light));
}

void LightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    setUniform(fragUnis::worldCameraPos, 1, cameraPos);
}

void LightingTechnique::setCameraViewMat(const glm::mat4& viewMat) {
    setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, viewMat);
}

void LightingTechnique::setMaterialSpecularIntensity(const float intensity) {
    setUniform(fragUnis::materialSpecularIntensity, intensity);
}

void LightingTechnique::setMaterialShininess(const float shininess) {
    setUniform(fragUnis::materialShininess, shininess);
}

void LightingTechnique::setShadowMapData(ShadowRenderer& shadowRenderer) {
    for (unsigned int i = 0; i < shadowRenderer.numCascades; i++) {
        setUniform(fragUnis::lightSpaceMatrices[i], 1, GL_FALSE, shadowRenderer.getCascade(i).getLightSpaceMatrix());
        setUniform(fragUnis::cascadeFarPlanes[i], shadowRenderer.getCascade(i).zFar);
    }

    for (unsigned int i = 0; i < shadowRenderer.numPointBuffers; i++) {
        setUniform(fragUnis::cubeMapFarPlanes[i], shadowRenderer.getPointBuffer(i).farPlane);
    }
}

void LightingTechnique::setFogColor(const glm::vec3& color) {
    setUniform(fragUnis::fogColor, 1, color);
}