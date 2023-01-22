#pragma once

#include "Debug/Debug.h"

#include <glm/glm.hpp>

#include "GLSLCPPBinder.h"
#include "Model/Storage/Vertex.h"

class Point {
public:
	Point(glm::vec3 point) : aPos(point) {
		LOG::CTOR::debug(this, &aPos, DY::std_format("Point created with value: {}", DY::glm_to_string(aPos)));
	}
	Point(float x, float y, float z) {
		aPos = glm::vec3(x, y, z);
		LOG::CTOR::debug(this, &aPos, DY::std_format("Point created with value: {}", DY::glm_to_string(aPos)));
	}
	Point operator=(glm::vec3 point) const {
		return { point };
	}


	decltype(Binder::line_vertex::locations::aPos)::type aPos;


	inline static LocationVector locInfo = {
		Binder::line_vertex::locations::aPos
	};

	inline static LocationVector locDivisors = {
		//Binder::geometry_vertex::locations::instanceMatrix
	};

	
	DY::ObjectRegister<Point,
		decltype(aPos),
		decltype(locInfo),
		decltype(locDivisors)> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<DY::No_CB, decltype(OB), DY::No_FB, DY::No_VB>;
};
