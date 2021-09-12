#pragma once
#include "Technique.h"
#include "LightTypes.h"
#include "ShadowRenderer.h"

 class LightingTechnique : public Technique
{

public:
	using Technique::Technique;

	virtual void init();
	virtual void setModel(const glm::mat4& modelMat) {};

	void setDirectionalLight(const GLSLDirectionalLight& light);
	void setPointLight(const GLSLPointLight& light, const int& index);
	void setSpotLight(const GLSLSpotLight& light);

	void setWorldCameraPos(const glm::vec3& cameraPos);
	void setCameraViewMat(const glm::mat4& viewMat);
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialShininess(const float shininess);

	void setShadowMapData(ShadowRenderer& shadowRenderer);

	void setFogColor(const glm::vec3& color);

private:
};

