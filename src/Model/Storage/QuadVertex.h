#pragma once

#include <glm/glm.hpp>

#include "Vertex.h"
#include "BufferLayout.h"

class QuadVertex : public Vertex {
public:
	QuadVertex() : aPos(0.0), aTexCoords(0.0) {}

	QuadVertex(glm::vec3 position, glm::vec2 texCoords) : aPos(position), aTexCoords(texCoords) {}

	glm::vec3 aPos;
	glm::vec2 aTexCoords;

	static BufferLayout& getFormat() {
		return format;
	}
	
private:
	inline static BufferLayout format = {
		{ "aPos", ShaderDataType::Float3 },
		{ "aTexCoords", ShaderDataType::Float2 },
	};
};
