#include "Renderable.h"

void Renderable::render()
{
	renderingLogic();
	//glBindVertexArray(vertexBuffer.getVAO());
	//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 64);
	glBindVertexArray(0);

	//Should be moved to where it makes sense
	glActiveTexture(GL_TEXTURE0);
}
