#pragma once
#include "Technique.h"
 class LightingTechnique : public Technique
{
	 struct DirectionalLight
	 {
		 float ambientIntensity;
		 glm::vec3 color;
		 float diffuseIntensity;
		 glm::vec3 direction;
	 };

public:
	LightingTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode);

	void setModel(const glm::mat4& model);
	void setView(const glm::mat4& view);
	void setProjection(const glm::mat4& projection);

	void setTextureUnit(GLuint textureUnit);
	void setDirectionalLight(const DirectionalLight& light);

	void setWorldCameraPos(const glm::vec3& cameraPos);
	void setMaterialSpecularIntensity(const float intensity);
	void setMaterialSpecularPower(const float power);

private:
};

