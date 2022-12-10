#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"

class DeferredLightingTechnique : public ShaderProgram<DeferredShader> {
public:
	using fragUnis = Binder::deferred_frag::uniforms;
	static void init();

	static void setDirectionalLight(const DirectionalLight& dirLight);
	static void setPointLight(const PointLight& pointLight, int index);
	static void setSpotLight(const SpotLight& light, int index);

	static void setWorldCameraPos(const glm::vec3& cameraPos);
	static void setCameraViewMat(const glm::mat4& view);
	static void setInverseViewMat(const glm::mat4& inverseView);

	//void setShadowMapData(ShadowRenderer& shadowRenderer);

	static void setFogColor(const glm::vec3& color);

	// uniform buffer upload
	static void uploadMaterialData();
	static void uploadSpotLightData(const std::vector<SpotLight>& lightVector);
	static void uploadPointLightData(const std::vector<PointLight>& lightVector);

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::deferred_frag::buffers::MaterialData);
		return materialData;
	}

	static UniformBuffer& getUBSpotLightData() {
		static UniformBuffer spotLightData = UniformBuffer(Binder::deferred_frag::buffers::SpotLightData);
		return spotLightData;
	}

	static UniformBuffer& getUBPointLightData() {
		static UniformBuffer pointLightData = UniformBuffer(Binder::deferred_frag::buffers::PointLightData);
		return pointLightData;
	}
};

