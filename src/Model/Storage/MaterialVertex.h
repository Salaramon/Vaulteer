#pragma once

#include "VertexImpl.h"

// used by batch
class MaterialVertex : public Vertex {
public:
	MaterialVertex() : aPos(0.0), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0) {}

	MaterialVertex(const VertexImpl& v, int materialNumber) :
			aPos(v.aPos), aNormal(v.aNormal), aTexCoords(v.aTexCoords), aTangent(v.aTangent), aBitangent(v.aBitangent), aMaterialNumber(materialNumber) {}

	MaterialVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent, int materialNumber) :
			aPos(pos), aNormal(normal), aTexCoords(texCoords), aTangent(tangent), aBitangent(bitangent), aMaterialNumber(materialNumber) {}

	glm::vec3 aPos;
	glm::vec3 aNormal;
	glm::vec2 aTexCoords;
	glm::vec3 aTangent;
	glm::vec3 aBitangent;
	int aMaterialNumber;

	static VertexBufferLayout& getFormat() {
		return format;
	}
	
private:
	inline static VertexBufferLayout format = {
		{ "aPos", ShaderDataType::Float3 },
		{ "aNormal", ShaderDataType::Float3 },
		{ "aTexCoords", ShaderDataType::Float2 },
		{ "aTangent", ShaderDataType::Float3 },
		{ "aBitangent", ShaderDataType::Float3 },
		{ "aMaterialNumber", ShaderDataType::Int },
		{ "instance", ShaderDataType::Mat4, 1 } // instanced
	};
};
