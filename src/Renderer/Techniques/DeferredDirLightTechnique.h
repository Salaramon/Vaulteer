#pragma once

#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"

class DeferredDirLightTechnique {
public:
	using dirVertUnis = Binder::deferred_directional_vertex::uniforms;
	using dirFragUnis = Binder::deferred_directional_frag::uniforms;

	static void setWorldCameraPos(const glm::vec3& cameraPos);
	static void setCameraViewMat(const glm::mat4& view);

	//void setShadowMapData(ShadowRenderer& shadowRenderer);

	static void setFogColor(const glm::vec3& color);

	// uniform buffer upload
	static void uploadMaterialData();
	static void uploadDirectionalLightData(const std::vector<DirectionalLight>& lightVector);

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::deferred_directional_frag::buffers::MaterialData);
		return materialData;
	}

	static UniformBuffer& getUBDirectionalLightData() {
		static UniformBuffer dirLightData = UniformBuffer(Binder::deferred_directional_frag::buffers::DirectionalLightData);
		return dirLightData;
	}

};

