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
	decltype(Binder::vertex::locations::aPos)::type aPos;
	decltype(Binder::vertex::locations::aNormal)::type aNormal;
	decltype(Binder::vertex::locations::aTexCoords)::type aTexCoords;
};

