#pragma once

#include "Shader.h"

class Renderable
{
public:
	virtual void render(const Shader& shader) = 0;
};

