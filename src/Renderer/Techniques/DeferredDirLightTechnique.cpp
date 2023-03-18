#include "vpch.h"
#include "Renderer/Techniques/DeferredDirLightTechnique.h"

#include "Model/Resources/MaterialLibrary.h"


void DeferredDirLightTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    //shader->setUniform(fragUnis::worldCameraPos, 1, cameraPos);
}

void DeferredDirLightTechnique::setCameraViewMat(const glm::mat4& view) {
    //shader->setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, view);

    //setInverseViewMat(glm::inverse(view));
}

void DeferredDirLightTechnique::uploadMaterialData() {
	std::vector<Material::MaterialData> materials;
	for (const auto& pair : MaterialLibrary::getAllMaterials()) {
		materials.push_back(pair.second->data);
	}
	auto& materialData = getUBMaterialData();
	UniformBuffer::insert(materialData, materials);
}

void DeferredDirLightTechnique::uploadDirectionalLightData(const std::vector<DirectionalLight>& lightVector) {
	auto& dirLightBuffer = getUBDirectionalLightData();
	UniformBuffer::insert(dirLightBuffer, lightVector);
}

/*void LightingTechnique::setShadowMapData(ShadowRenderer& shadowRenderer) {
    for (unsigned int i = 0; i < shadowRenderer.numCascades; i++) {
        //shader->setUniform(fragUnis::lightSpaceMatrices[i], 1, GL_FALSE, shadowRenderer.getCascade(i).getLightSpaceMatrix());
        //shader->setUniform(fragUnis::cascadeFarPlanes[i], shadowRenderer.getCascade(i).zFar);
    }

    for (unsigned int i = 0; i < shadowRenderer.numPointBuffers; i++) {
        //shader->setUniform(fragUnis::cubeMapFarPlanes[i], shadowRenderer.getPointBuffer(i).farPlane);
    }
}*/

void DeferredDirLightTechnique::setFogColor(const glm::vec3& color) {
    //shader->setUniform(fragUnis::fogColor, 1, color);
}
