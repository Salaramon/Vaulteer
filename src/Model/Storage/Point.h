#pragma once

#include "Debug/Debug.h"

#include <glm/glm.hpp>

#include "Vertex.h"

class Point : Vertex {
public:
	Point() : aPos(0.0) {
		LOG::CTOR::debug(this, "Point constructor called");
	}

	Point(glm::vec3 p) : aPos(p) {
		LOG::CTOR::debug(this, "Point constructor called with point");
	}

	glm::vec3 aPos;

	static BufferLayout& getFormat() {
		return format;
	}
	
private:
	inline static BufferLayout format = {
		{"aPos", ShaderDataType::Float3 }
	};

	DY::ObjectRegister<Point> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<DY::No_CB, decltype(OB), DY::No_FB, DY::No_VB>;

};
