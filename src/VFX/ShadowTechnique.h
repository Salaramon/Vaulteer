#pragma once

#include "Renderer/Techniques/Technique.h"

class ShadowTechnique : public Technique {
public:
	using Technique::Technique;

	virtual void setModel(const glm::mat4& model);
	void setLightSpaceMatrix(const glm::mat4& lightSpaceMat);

private:
};
