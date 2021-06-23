#pragma once
#include "Technique.h"
#include "LightTypes.h"

 class LightingTechnique : public Technique
{

public:
	LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode);

	void setModel(const glm::mat4& model);
	void setView(const glm::mat4& view);
	void setProjection(const glm::mat4& projection);

	void setTextureUnit(GLuint textureUnit);
	void setDirectionalLight(const DirectionalLight& light);
	void setPointLight(const PointLight& light);

	void setWorldCameraPos(const glm::vec3& cameraPos);
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialSpecularPower(const float power);

private:
};

