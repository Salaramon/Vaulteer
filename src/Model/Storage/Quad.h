#pragma once

#include <array>

#include "Model/Storage/QuadVertex.h"
#include "Utils/TypeDefUtils.h"

struct Quad {
	float x, y, w, h;
	glm::vec2 texFrom, texTo;

	Quad(float x, float y, float w, float h, glm::vec2 texFrom = {0.0, 0.0}, glm::vec2 texTo = {1.0, 1.0})
		: x(x), y(y), w(w), h(h), texFrom(texFrom), texTo(texTo) {}

	std::array<QuadVertex, 4> vertices() {
		std::array<QuadVertex, 4> vertices;
		vertices[0] = QuadVertex({x, y, 0}, texFrom);
		vertices[1] = QuadVertex({x + w, y, 0}, glm::vec2(texTo.x, texFrom.y));
		vertices[2] = QuadVertex({x + w, y - h, 0}, texTo);
		vertices[3] = QuadVertex({x, y - h, 0}, glm::vec2(texFrom.x, texTo.y));
		return vertices;
	}

	std::array<uint, 6> indices() {
		return {0, 1, 2, 2, 3, 0};
	}
};
