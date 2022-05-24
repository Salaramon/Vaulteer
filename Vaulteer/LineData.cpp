#include "vpch.h"
#include "LineData.h"

LineData::LineData(std::vector<Point> lines) : 
	vertexBuffer(storePointsAndIndices<PointHash>(indices, points, lines)),
	color(1, 1, 1, 1),
	pointCount(lines.size()),
	elementBuffer(indices, vertexBuffer.getVAO())
{}

void LineData::draw(const Shader& shader)
{
	vertexBuffer.getVAO().bind();
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	vertexBuffer.getVAO().unbind();
}

void LineData::setColor(glm::vec4 color)
{
	LineData::color = color;
}

glm::vec4 LineData::getColor()
{
	return color;
}
