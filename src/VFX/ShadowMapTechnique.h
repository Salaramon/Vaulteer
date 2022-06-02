#pragma once

#include "Technique.h"

class ShadowMapTechnique : public Technique {
public:
	using Technique::Technique;

	void setDepthMap(const GLuint depthMapTexId);

private:
};
