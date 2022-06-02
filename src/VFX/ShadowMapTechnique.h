#pragma once

#include "Renderer/Techniques/Technique.h"

class ShadowMapTechnique : public Technique {
public:
	using Technique::Technique;

	void setDepthMap(const GLuint depthMapTexId);

private:
};
