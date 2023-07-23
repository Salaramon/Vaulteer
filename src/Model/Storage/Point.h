#pragma once

#include <glm/glm.hpp>

#include "VertexBufferLayout.h"
#include "Vertex.h"

class Point : Vertex {
public:
	Point() : aPos(0.0) {}

	Point(glm::vec3 p) : aPos(p) {}

	glm::vec3 aPos;

	static VertexBufferLayout& getFormat() {
		return format;
	}
	
private:
	inline static VertexBufferLayout format = {
		{"aPos", ShaderDataType::Float3 }
	};
};
