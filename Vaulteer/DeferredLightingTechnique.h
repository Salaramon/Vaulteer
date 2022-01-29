#pragma once
#include "ShaderProgram.h"
#include "LightTypes.h"

 class DeferredLightingTechnique : public ShaderProgram<DeferredShader>
{

public:
	using fragUnis = Binder::deferred_frag::uniforms;

	virtual void init();
	virtual void setModel(const glm::mat4& modelMat) {};

	void setDirectionalLight(const DirectionalLight& light);
	void setPointLight(const PointLight& light, const int index);
	void setSpotLight(const SpotLight& light, const int index);

	void setWorldCameraPos(const glm::vec3& cameraPos);
	void setCameraViewMat(const glm::mat4& viewMat);
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialShininess(const float shininess);

	//void setShadowMapData(ShadowRenderer& shadowRenderer);

	void setFogColor(const glm::vec3& color);



private:
};

