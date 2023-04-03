#pragma once

#include "Vertex.h"
#include "BufferLayout.h"

class MaterialVertex : public Vertex {
public:
	MaterialVertex() : aPos(0.0), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0), aMaterialNumber(0) {}

	MaterialVertex(glm::vec3 point) : aPos(point), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0), aMaterialNumber(0) {}

	glm::vec3 aPos;
	glm::vec3 aNormal;
	glm::vec2 aTexCoords;
	glm::vec3 aTangent;
	glm::vec3 aBitangent;
	int aMaterialNumber;

	static BufferLayout& getFormat() {
		return format;
	}
	
private:
	inline static BufferLayout format = {
		{"aPos", ShaderDataType::Float3 },
		{"aNormal", ShaderDataType::Float3 },
		{"aTexCoords", ShaderDataType::Float2 },
		{"aTangent", ShaderDataType::Float3 },
		{"aBitangent", ShaderDataType::Float3 },
		{"aMaterialNumber", ShaderDataType::Int }
	};
};
