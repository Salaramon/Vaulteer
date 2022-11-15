#pragma once

#include "Debug/Debug.h"

#include "GLSLCPPBinder.h"

typedef std::vector<Binder::LocationInfo> LocationVector;

class Vertex
{
public:
	Vertex(glm::vec3 point) {
		LOG::CTOR::debug(this, "Vertex constructor called");
		aPos = point;
	}
	Vertex() {
		LOG::CTOR::debug(this, "Vertex constructor called");
	}
	decltype(Binder::geometry_vertex::locations::aPos)::type aPos;
	decltype(Binder::geometry_vertex::locations::aNormal)::type aNormal;
	decltype(Binder::geometry_vertex::locations::aTexCoords)::type aTexCoords;
	decltype(Binder::geometry_vertex::locations::aTangent)::type aTangent;
	decltype(Binder::geometry_vertex::locations::aBitangent)::type aBitangent;
	decltype(Binder::geometry_vertex::locations::aModelNumber)::type aModelNumber;


	inline static LocationVector locInfo = {
		Binder::geometry_vertex::locations::aPos,
		Binder::geometry_vertex::locations::aNormal,
		Binder::geometry_vertex::locations::aTexCoords,
		Binder::geometry_vertex::locations::aTangent,
		Binder::geometry_vertex::locations::aBitangent,
		Binder::geometry_vertex::locations::aModelNumber
	};

	inline static LocationVector locDivisors = {
		//Binder::geometry_vertex::locations::instanceMatrix
	};


	DY::ObjectRegister<Vertex,
		decltype(aPos),
		decltype(aNormal),
		decltype(aTexCoords),
		decltype(aTangent),
		decltype(aBitangent),
		decltype(aModelNumber),
		decltype(locInfo),
		decltype(locDivisors)> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<DY::No_CB, decltype(OB), DY::No_FB, DY::No_VB>;
};

