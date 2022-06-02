#include "vpch.h"
#include "Model/Data/LineData.h"


LineData::LineData(glm::vec4 color, std::vector<Point> lines) :
	vertexBuffer(storePointsAndIndices<PointHash>(indices, points, lines)),
	color(color),
	pointCount(lines.size()),
	elementBuffer(indices, vertexBuffer.getVAO())
{}

void LineData::setColor(glm::vec4 color)
{
	LineData::color = color;
}

glm::vec4 LineData::getColor()
{
	return color;
}
