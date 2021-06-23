#pragma once

#include <string>
#include <vector>
#include <array>

#include "VertexArray.h"
#include "GLSLCPPBinder.h"

class VertexAttribute
{
public:
	template<const size_t arraySize>
	VertexAttribute(VertexArray& vertexArray, std::array<Binder::Location, arraySize> location) {
		vertexArray.bind();

		for (size_t i = 0; i < location.size(); i++) {
			glEnableVertexAttribArray(location[i].id);
			glVertexAttribPointer(location[i].id, location[i].size, GL_FLOAT, GL_FALSE, sizeof(Vertex), Vertex::offset[location.id]);
		}

		vertexArray.unbind();
	}

	VertexAttribute(VertexArray& vertexArray) {

		vertexArray.bind();

		size_t size = 0;
		for (size_t i = 0; i < locations.size(); i++) {
			glEnableVertexAttribArray(locations[i].id);
			glVertexAttribPointer(locations[i].id, locations[i].size, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(size * sizeof(float)));
			size += locations[i].size;
		}

		vertexArray.unbind();
	}

private:
	inline static const std::array<Binder::Location, 3> locations = {
			Binder::vertex::locations::aPos
			,Binder::vertex::locations::aNormal
			,Binder::vertex::locations::aTexCoords
	};
};