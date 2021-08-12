#pragma once
#include "Technique.h"
#include "LightTypes.h"

 class LightingTechnique : public Technique
{

public:
	LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode);

	virtual void init();

	void setDirectionalLight(const GLSLDirectionalLight& light);
	void setPointLight(const GLSLPointLight& light, const int& index);
	void setSpotLight(const GLSLSpotLight& light);

	void setWorldCameraPos(const glm::vec3& cameraPos);
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialShininess(const float shininess);

private:
};

