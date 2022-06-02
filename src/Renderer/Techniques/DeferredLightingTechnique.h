#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/UniformBuffer.h"

#include "VFX/LightTypes.h"
#include "Model/Material.h"

class DeferredLightingTechnique : public ShaderProgram<DeferredShader> {
public:
	using fragUnis = Binder::deferred_frag::uniforms;
	static void init();

	static void setDirectionalLight(const DirectionalLight& light);
	static void setPointLight(const PointLight& light, const int index);
	static void setSpotLight(const SpotLight& light, const int index);

	static void setWorldCameraPos(const glm::vec3& cameraPos);
	static void setCameraViewMat(const glm::mat4& viewMat);
	static void setMaterialSpecularIntensity(const float intensity);
	static void setMaterialShininess(const float shininess);

	//void setShadowMapData(ShadowRenderer& shadowRenderer);

	static void setFogColor(const glm::vec3& color);

	// uniform buffer upload
	static void uploadMaterialData(const std::vector<ModelData*>& modelVector);

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::deferred_frag::buffers::MaterialData);
		return materialData;
	}
};

