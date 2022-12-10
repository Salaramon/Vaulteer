#include "vpch.h"
#include "Renderer/Techniques/DeferredLightingTechnique.h"

#include "Model/Resources/MaterialLibrary.h"

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

void DeferredLightingTechnique::setDirectionalLight(const DirectionalLight& dirLight) {
    shader->setUniform(fragUnis::directionalLight.light.color, 1,dirLight.light.color);
    shader->setUniform(fragUnis::directionalLight.light.ambientIntensity, dirLight.light.ambientIntensity);
    shader->setUniform(fragUnis::directionalLight.light.diffuseIntensity, dirLight.light.diffuseIntensity);

    shader->setUniform(fragUnis::directionalLight.direction, 1, glm::normalize(dirLight.direction));
}

void DeferredLightingTechnique::setPointLight(const PointLight& pointLight, const int index) {
    shader->setUniform(fragUnis::pointLights[index].light.color, 1, pointLight.light.color);
    shader->setUniform(fragUnis::pointLights[index].light.ambientIntensity, pointLight.light.ambientIntensity);
    shader->setUniform(fragUnis::pointLights[index].light.diffuseIntensity, pointLight.light.diffuseIntensity);

    shader->setUniform(fragUnis::pointLights[index].att.aConstant, pointLight.attenuation.constant);
    shader->setUniform(fragUnis::pointLights[index].att.aLinear, pointLight.attenuation.linear);
    shader->setUniform(fragUnis::pointLights[index].att.aQuadratic, pointLight.attenuation.quadratic);

    shader->setUniform(fragUnis::pointLights[index].radius, pointLight.calculatePointRadius());

    shader->setUniform(fragUnis::pointLights[index].position, 1, pointLight.position);
}

void DeferredLightingTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader->setUniform(fragUnis::worldCameraPos, 1, cameraPos);
}

void DeferredLightingTechnique::setCameraViewMat(const glm::mat4& view) {
    shader->setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, view);

    setInverseViewMat(glm::inverse(view));
}

void DeferredLightingTechnique::setInverseViewMat(const glm::mat4& inverseViewMat) {
    shader->setUniform(fragUnis::inverseViewMat, 1, GL_FALSE, inverseViewMat);
}

void DeferredLightingTechnique::uploadMaterialData() {
	std::vector<Material::MaterialData> materials;
	for (const auto& pair : MaterialLibrary::getAllMaterials()) {
		materials.push_back(pair.second->data);
	}
	auto& materialData = getUBMaterialData();
	UniformBuffer::insert(materialData, materials);
}

void DeferredLightingTechnique::uploadSpotLightData(const std::vector<SpotLight>& lightVector) {
	auto& spotLightBuffer = getUBSpotLightData();
	UniformBuffer::insert(spotLightBuffer, lightVector);
}

void DeferredLightingTechnique::uploadPointLightData(const std::vector<PointLight>& lightVector) {
	auto& pointLightBuffer = getUBPointLightData();
	UniformBuffer::insert(pointLightBuffer, lightVector);
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