#pragma once

#include "VertexBuffer.h"
#include "Texture.h"

class Renderable
{
public:
	void render();

protected:
	virtual void renderingLogic() = 0;
	//VertexBuffer vertexBuffer;
private:
	
	Texture texture;
};

