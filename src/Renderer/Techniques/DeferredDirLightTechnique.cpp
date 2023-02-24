#include "vpch.h"
#include "Renderer/Techniques/DeferredDirLightTechnique.h"

#include "Model/Resources/MaterialLibrary.h"


void DeferredDirLightTechnique::setWorldCameraPos(const glm::vec3& cameraPos) {
    shader->setUniform(dirFragUnis::worldCameraPos, 1, cameraPos);
}

void DeferredDirLightTechnique::setCameraViewMat(const glm::mat4& view) {
    shader->setUniform(dirFragUnis::cameraViewMat, 1, GL_FALSE, view);

    //setInverseViewMat(glm::inverse(view));
}

void DeferredDirLightTechnique::uploadMaterialData() {
	std::vector<Material::MaterialData> materials;
	for (const auto& val : MaterialLibrary::getAllMaterials() | std::views::values) {
		materials.push_back(val->data);
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
        shader->setUniform(fragUnis::lightSpaceMatrices[i], 1, GL_FALSE, shadowRenderer.getCascade(i).getLightSpaceMatrix());
        shader->setUniform(fragUnis::cascadeFarPlanes[i], shadowRenderer.getCascade(i).zFar);
    }

    for (unsigned int i = 0; i < shadowRenderer.numPointBuffers; i++) {
        shader->setUniform(fragUnis::cubeMapFarPlanes[i], shadowRenderer.getPointBuffer(i).farPlane);
    }
}*/

void DeferredDirLightTechnique::setFogColor(const glm::vec3& color) {
    shader->setUniform(dirFragUnis::fogColor, 1, color);
}
