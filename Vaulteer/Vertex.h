#pragma once

#include "GLSLCPPBinder.h"
#include "DebugLogger.h"

class Vertex
{
public:
	Vertex() {
		DebugLogger<Vertex> log;
		log.debug("Vertex created.\n");

	}
	decltype(Binder::geometry_vertex::locations::aPos)::type aPos;
	decltype(Binder::geometry_vertex::locations::aNormal)::type aNormal;
	decltype(Binder::geometry_vertex::locations::aTexCoords)::type aTexCoords;
};

