#pragma once

#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"
#include "Model/Resources/MaterialLibrary.h"

class DeferredPointLightTechnique {
public:
	using vertUnis = Binder::deferred_point_vertex::uniforms;
	using fragUnis = Binder::deferred_point_frag::uniforms;
	
	/*
	TODO: replace this with some kind of texture handling technique
*/
	static void init() {
		int texId = 0;

		//shader->setUniform(fragUnis::gPosition, texId++);
		//shader->setUniform(fragUnis::gNormal, texId++);
		//shader->setUniform(fragUnis::gColor, texId++);
		//shader->setUniform(fragUnis::shadowMap_0, texId++);
		//shader->setUniform(fragUnis::shadowMap_1, texId++);
		//shader->setUniform(fragUnis::shadowMap_2, texId++);
		//shader->setUniform(fragUnis::shadowCubeMap_0, texId++);
		//shader->setUniform(fragUnis::shadowCubeMap_1, texId++);
		//shader->setUniform(fragUnis::shadowCubeMap_2, texId++);
		//shader->setUniform(fragUnis::shadowCubeMap_3, texId++);
		//shader->setUniform(fragUnis::shadowSpotMap_0, texId++);
		//shader->setUniform(fragUnis::shadowSpotMap_1, texId++);
		//shader->setUniform(fragUnis::shadowSpotMap_2, texId++);
		//shader->setUniform(fragUnis::shadowSpotMap_3, texId++);
	}


	static void setPointLightIndex(const int index) {
		//shader->setUniform(fragUnis::pointLightIndex, index);
	}

	static void setModel(const glm::dmat4& model) {
		//shader->setUniform(vertUnis::model, 1, GL_FALSE, model);
	}

	static void setView(const glm::dmat4& view) {
		//shader->setUniform(vertUnis::view, 1, GL_FALSE, view);
	}

	static void setWorldCameraPos(const glm::vec3& cameraPos) {
		//shader->setUniform(fragUnis::worldCameraPos, 1, cameraPos);
	}

	static void setCameraViewMat(const glm::mat4& view) {
		////shader->setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, view);

		//setInverseViewMat(glm::inverse(view));
	}

	static void uploadMaterialData() {
		std::vector<Material::MaterialData> materials;
		for (const auto& pair : MaterialLibrary::getAllMaterials()) {
			materials.push_back(pair.second->data);
		}
		auto& materialData = getUBMaterialData();
		UniformBuffer::insert(materialData, materials);
	}

	static void uploadSpotLightData(const std::vector<SpotLight>& lightVector) {
		auto& spotLightBuffer = getUBSpotLightData();
		UniformBuffer::insert(spotLightBuffer, lightVector);
	}

	static void uploadPointLightData(const std::vector<PointLight>& lightVector) {
		auto& pointLightBuffer = getUBPointLightData();
		UniformBuffer::insert(pointLightBuffer, lightVector);
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

	static void setFogColor(const glm::vec3& color) {
		////shader->setUniform(fragUnis::fogColor, 1, color);
	}

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::deferred_point_frag::buffers::MaterialData);
		return materialData;
	}

	static UniformBuffer& getUBSpotLightData() {
		static UniformBuffer spotLightData = UniformBuffer(Binder::deferred_point_frag::buffers::SpotLightData);
		return spotLightData;
	}

	static UniformBuffer& getUBPointLightData() {
		static UniformBuffer pointLightData = UniformBuffer(Binder::deferred_point_frag::buffers::PointLightData);
		return pointLightData;
	}

};

