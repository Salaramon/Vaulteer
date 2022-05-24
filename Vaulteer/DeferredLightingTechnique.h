#pragma once

#include "ShaderProgram.h"
#include "UniformBuffer.h"

#include "LightTypes.h"
#include "Material.h"

class DeferredLightingTechnique : public ShaderProgram<DeferredShader> {
public:
	using fragUnis = Binder::deferred_frag::uniforms;
	void init();

	void setDirectionalLight(const DirectionalLight& light);
	void setPointLight(const PointLight& light, const int index);
	void setSpotLight(const SpotLight& light, const int index);

	void setWorldCameraPos(const glm::vec3& cameraPos);
	void setCameraViewMat(const glm::mat4& viewMat);
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialShininess(const float shininess);

	//void setShadowMapData(ShadowRenderer& shadowRenderer);

	void setFogColor(const glm::vec3& color);

	// uniform buffer upload
	void uploadMaterialData(const std::vector<ModelData*>& modelVector);

	UniformBuffer materialData = UniformBuffer(Binder::deferred_frag::buffers::MaterialData);
};

