#pragma once

#include "VertexImpl.h"

// used by batch
class MaterialVertex : public Vertex {
public:
	MaterialVertex() : aPos(0.0), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0) {}

	MaterialVertex(const VertexImpl& v, int materialNumber) :
			aPos(v.aPos), aNormal(v.aNormal), aTexCoords(v.aTexCoords), aTangent(v.aTangent), aBitangent(v.aBitangent), aMaterialNumber(materialNumber) {}

	MaterialVertex(const VertexImpl& v, glm::vec3 pos, glm::vec3 normal, int materialNumber) :
			aPos(pos), aNormal(normal), aTexCoords(v.aTexCoords), aTangent(v.aTangent), aBitangent(v.aBitangent), aMaterialNumber(materialNumber) {}

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
		{ "aPos", ShaderDataType::Float3 },
		{ "aNormal", ShaderDataType::Float3 },
		{ "aTexCoords", ShaderDataType::Float2 },
		{ "aTangent", ShaderDataType::Float3 },
		{ "aBitangent", ShaderDataType::Float3 },
		{ "aMaterialNumber", ShaderDataType::Int }
	};
};
