#include "vpch.h"
#include "Renderer/Techniques/DeferredPointLightTechnique.h"

#include "Model/Resources/MaterialLibrary.h"

/*
    TODO: replace this with some kind of texture handling technique
*/
void DeferredPointLightTechnique::init() {
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


void DeferredPointLightTechnique::setPointLightIndex(const int index) {
    shader->setUniform(fragUnis::pointLightIndex, index);
}

void DeferredPointLightTechnique::setModel(const glm::dmat4& model) {
	shader->setUniform(vertUnis::model, 1, GL_FALSE, model);
}

void DeferredPointLightTechnique::setView(const glm::dmat4& view) {
	shader->setUniform(vertUnis::view, 1, GL_FALSE, view);
}

void DeferredPointLightTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader->setUniform(fragUnis::worldCameraPos, 1, cameraPos);
}

void DeferredPointLightTechnique::setCameraViewMat(const glm::mat4& view) {
    shader->setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, view);

    //setInverseViewMat(glm::inverse(view));
}

void DeferredPointLightTechnique::uploadMaterialData() {
	std::array<Material::MaterialData, 128> materials;
	for (const auto& val : MaterialLibrary::getAllMaterials() | std::views::values) {
		materials[val->materialIndex] = val->data;
	}
	auto& materialData = getUBMaterialData();
	UniformBuffer::insert(materialData, materials);
}

void DeferredPointLightTechnique::uploadSpotLightData(const std::vector<SpotLight>& lightVector) {
	auto& spotLightBuffer = getUBSpotLightData();
	UniformBuffer::insert(spotLightBuffer, lightVector);
}

void DeferredPointLightTechnique::uploadPointLightData(const std::vector<PointLight>& lightVector) {
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

void DeferredPointLightTechnique::setFogColor(const glm::vec3& color) {
    shader->setUniform(fragUnis::fogColor, 1, color);
}