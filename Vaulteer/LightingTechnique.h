#pragma once
#include "Technique.h"
#include "LightTypes.h"

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
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialShininess(const float shininess);

	void setLightSpaceMatrix(const glm::mat4 &lightSpaceMatrix);

private:
};

