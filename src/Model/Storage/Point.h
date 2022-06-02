#pragma once

#include <glm/glm.hpp>

#include "GLSLCPPBinder.h"
#include "Vertex.h"

class Point
{
public:
	Point(glm::vec3 point) : aPos(point) {
	}
	Point(float x, float y, float z) {
		aPos = glm::vec3(x, y, z);
	}
	Point operator=(glm::vec3 point) {
		return Point(point);
	}


	decltype(Binder::line_vertex::locations::aPos)::type aPos;


	inline static LocationVector locInfo = {
		Binder::line_vertex::locations::aPos
	};

	inline static LocationVector locDivisors = {
		//Binder::geometry_vertex::locations::instanceMatrix
	};
};

