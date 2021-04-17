#include "Renderer.h"

void Renderer::render(Renderable& renderable)
{
	renderable.render();
	//glDrawElements(GL_TRIANGLES, vertexBuffer.getNumberOfIndicies(), GL_UNSIGNED_INT, 0);
}
