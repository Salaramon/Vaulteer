#pragma once

#include <glm/glm.hpp>

#include "BufferLayout.h"
#include "Vertex.h"

class Point : Vertex {
public:
	Point() : aPos(0.0) {}

	Point(glm::vec3 p) : aPos(p) {}

	glm::vec3 aPos;

	static BufferLayout& getFormat() {
		return format;
	}
	
private:
	inline static BufferLayout format = {
		{"aPos", ShaderDataType::Float3 }
	};
};
