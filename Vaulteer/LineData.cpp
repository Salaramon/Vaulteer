#include "LineData.h"

LineData::LineData(glm::vec4 color, std::vector<Point> lines) :
	vertexArray(),
	vertexBuffer(storePointsAndIndices<PointHash>(indices, points, lines), vertexArray, locInfo),
	color(color),
	pointCount(lines.size()),
	elementBuffer(indices, vertexArray)
{}

void LineData::draw(const Shader& shader)
{
	vertexArray.bind();
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	vertexArray.unbind();
}

void LineData::setColor(glm::vec4 color)
{
	LineData::color = color;
}

glm::vec4 LineData::getColor()
{
	return color;
}
