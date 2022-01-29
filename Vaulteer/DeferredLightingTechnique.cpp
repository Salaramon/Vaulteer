#include "DeferredLightingTechnique.h"

/*
    TODO: replace this with some kind of texture handling technique
*/
void DeferredLightingTechnique::init() {
    int texId = 0;

    shader->setUniform(fragUnis::gPosition, texId++);
    shader->setUniform(fragUnis::gNormal, texId++);
    shader->setUniform(fragUnis::gColor, texId++);
    shader->setUniform(fragUnis::shadowMap_0, texId++);
    shader->setUniform(fragUnis::shadowMap_1, texId++);
    shader->setUniform(fragUnis::shadowMap_2, texId++);
    shader->setUniform(fragUnis::shadowCubeMap_0, texId++);
    shader->setUniform(fragUnis::shadowCubeMap_1, texId++);
    shader->setUniform(fragUnis::shadowCubeMap_2, texId++);
    shader->setUniform(fragUnis::shadowCubeMap_3, texId++);
    shader->setUniform(fragUnis::shadowSpotMap_0, texId++);
    shader->setUniform(fragUnis::shadowSpotMap_1, texId++);
    shader->setUniform(fragUnis::shadowSpotMap_2, texId++);
    shader->setUniform(fragUnis::shadowSpotMap_3, texId++);
}

void DeferredLightingTechnique::setDirectionalLight(const DirectionalLight& light) {
    shader->setUniform(fragUnis::directionalLight.light.color, 1,light.color);
    shader->setUniform(fragUnis::directionalLight.light.ambientIntensity, light.ambientIntensity);
    shader->setUniform(fragUnis::directionalLight.light.diffuseIntensity, light.diffuseIntensity);

    shader->setUniform(fragUnis::directionalLight.direction, 1, glm::normalize(light.direction));
}

void DeferredLightingTechnique::setPointLight(const PointLight& light, const int index) {
    shader->setUniform(fragUnis::pointLights[index].light.color, 1, light.color);
    shader->setUniform(fragUnis::pointLights[index].light.ambientIntensity, light.ambientIntensity);
    shader->setUniform(fragUnis::pointLights[index].light.diffuseIntensity, light.diffuseIntensity);

    shader->setUniform(fragUnis::pointLights[index].att.aConstant, light.attenuation.constant);
    shader->setUniform(fragUnis::pointLights[index].att.aLinear, light.attenuation.linear);
    shader->setUniform(fragUnis::pointLights[index].att.aQuadratic, light.attenuation.quadratic);

    shader->setUniform(fragUnis::pointLights[index].radius, light.calculatePointRadius());

    shader->setUniform(fragUnis::pointLights[index].position, 1, light.position);
}

void DeferredLightingTechnique::setSpotLight(const SpotLight& light, const int index) {
    shader->setUniform(fragUnis::spotLights[index].light.color, 1, light.color);
    shader->setUniform(fragUnis::spotLights[index].light.ambientIntensity, light.ambientIntensity);
    shader->setUniform(fragUnis::spotLights[index].light.diffuseIntensity, light.diffuseIntensity);

    shader->setUniform(fragUnis::spotLights[index].att.aConstant, light.attenuation.constant);
    shader->setUniform(fragUnis::spotLights[index].att.aLinear, light.attenuation.linear);
    shader->setUniform(fragUnis::spotLights[index].att.aQuadratic, light.attenuation.quadratic);

    shader->setUniform(fragUnis::spotLights[index].position, 1, light.position);

    shader->setUniform(fragUnis::spotLights[index].direction, 1, glm::normalize(light.direction));
    shader->setUniform(fragUnis::spotLights[index].angle, light.cutoffAngle);

    shader->setUniform(fragUnis::spotLights[index].radius, light.calculatePointRadius());
    shader->setUniform(fragUnis::spotLightSpaceMatrices[index], 1, GL_FALSE, light.getLightSpaceMatrix());
}

void DeferredLightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader->setUniform(fragUnis::worldCameraPos, 1, cameraPos);
}

void DeferredLightingTechnique::setCameraViewMat(const glm::mat4& viewMat) {
    shader->setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, viewMat);
}

void DeferredLightingTechnique::setMaterialSpecularIntensity(const float intensity) {
    shader->setUniform(fragUnis::materialSpecularIntensity, intensity);
}

void DeferredLightingTechnique::setMaterialShininess(const float shininess) {
    shader->setUniform(fragUnis::materialShininess, shininess);
}

/*void LightingTechnique::setShadowMapData(ShadowRenderer& shadowRenderer) {
    for (unsigned int i = 0; i < shadowRenderer.numCascades; i++) {
        shader->setUniform(fragUnis::lightSpaceMatrices[i], 1, GL_FALSE, shadowRenderer.getCascade(i).getLightSpaceMatrix());
        shader->setUniform(fragUnis::cascadeFarPlanes[i], shadowRenderer.getCascade(i).zFar);
    }

    for (unsigned int i = 0; i < shadowRenderer.numPointBuffers; i++) {
        shader->setUniform(fragUnis::cubeMapFarPlanes[i], shadowRenderer.getPointBuffer(i).farPlane);
    }
}*/

void DeferredLightingTechnique::setFogColor(const glm::vec3& color) {
    shader->setUniform(fragUnis::fogColor, 1, color);
}