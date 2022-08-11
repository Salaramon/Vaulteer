#pragma once

#include "GLSLCPPBinder.h"

typedef std::vector<Binder::LocationInfo> LocationVector;

class Vertex
{
public:
	Vertex(glm::vec3 point) {
		aPos = point;
	}
	Vertex() {
		//log.debug("Vertex created.\n");
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
};

