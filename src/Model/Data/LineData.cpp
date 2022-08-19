#include "vpch.h"
#include "Model/Data/LineData.h"


LineData::LineData(glm::vec4 color, std::vector<Point> lines) :
	vertexBuffer(storePointsAndIndices<PointHash>(indices, points, lines)),
	elementBuffer(indices, vertexBuffer.getVAO()),
	color(color),
	pointCount(lines.size()) {}

void LineData::setColor(glm::vec4 color) {
	LineData::color = color;
}

glm::vec4 LineData::getColor() const {
	return color;
}
