#pragma once

#include "Renderer/Techniques/Technique.h"

class DepthMapTechnique : public Technique {
public:
	using Technique::Technique;

	void setDepthMap(GLuint depthMapTexId);

private:
};
