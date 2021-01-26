#pragma once

#include "Shader.h"

class Renderable
{
public:
	virtual void draw(Shader  & shader) = 0;

private:
};

