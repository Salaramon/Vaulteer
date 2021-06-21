#include "Mesh.h"
#include "Shader.h"

void Mesh::draw()
{
	
	glDrawElements(GL_TRIANGLES, vertexArray.indices.size(), GL_UNSIGNED_INT, 0);
}
