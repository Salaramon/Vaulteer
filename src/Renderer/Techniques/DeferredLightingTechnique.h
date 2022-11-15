#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"

class DeferredLightingTechnique : public ShaderProgram<DeferredShader> {
public:
	using fragUnis = Binder::deferred_frag::uniforms;
	static void init();

	static void setDirectionalLight(const DirectionalLight& light);
	static void setPointLight(const PointLight& light, int index);
	static void setSpotLight(const SpotLight& light, int index);

	static void setWorldCameraPos(const glm::vec3& cameraPos);
	static void setCameraViewMat(const glm::mat4& view);
	static void setInverseViewMat(const glm::mat4& inverseView);

	//void setShadowMapData(ShadowRenderer& shadowRenderer);

	static void setFogColor(const glm::vec3& color);

	// uniform buffer upload
	static void uploadMaterialData(const std::vector<ModelData*>& modelVector);

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::deferred_frag::buffers::MaterialData);
		return materialData;
	}
};

