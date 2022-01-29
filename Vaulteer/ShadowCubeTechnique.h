#pragma once
#include "Technique.h"
class ShadowCubeTechnique : public Technique {
public:
	using Technique::Technique;

	virtual void setModel(const glm::mat4& model);

	void setShadowMatrices(const std::vector<glm::mat4>& shadowMatrices);

	void setLightPos(const glm::vec3& lightPos);

	void setFarPlane(const float farPlane);
};

