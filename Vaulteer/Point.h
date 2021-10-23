#pragma once

#include <glm/glm.hpp>

#include "GLSLCPPBinder.h"

class Point
{
public:
	Point(glm::vec3 point) {
		aPos = point;
	}
	Point(float x, float y, float z) {
		aPos = glm::vec3(x, y, z);
	}
	

	decltype(Binder::line_vertex::locations::aPos)::type aPos;
};

