#pragma once

#include <glm/glm.hpp>

#include "Vertex.h"
#include "BufferLayout.h"
#include "VertexContainer.h"
#include "Utils/TypeDefUtils.h"

class VertexImpl : public Vertex {
public:
	VertexImpl() : aPos(0.0), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0) {}

	VertexImpl(glm::vec3 point) : aPos(point), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0) {}

	glm::vec3 aPos;
	glm::vec3 aNormal;
	glm::vec2 aTexCoords;
	glm::vec3 aTangent;
	glm::vec3 aBitangent;

	static BufferLayout& getFormat() {
		return format;
	}

	static BufferLayout& getNonInstancedFormat() {
		return nonInstancedFormat;
	}
	
private:
	inline static BufferLayout format = {
		{ "aPos", ShaderDataType::Float3 },
		{ "aNormal", ShaderDataType::Float3 },
		{ "aTexCoords", ShaderDataType::Float2 },
		{ "aTangent", ShaderDataType::Float3 },
		{ "aBitangent", ShaderDataType::Float3 },
		{ "aMaterialNumber", ShaderDataType::Int, 1 } // instanced
	};

	// used by batch
	inline static BufferLayout nonInstancedFormat = {
		{ "aPos", ShaderDataType::Float3 },
		{ "aNormal", ShaderDataType::Float3 },
		{ "aTexCoords", ShaderDataType::Float2 },
		{ "aTangent", ShaderDataType::Float3 },
		{ "aBitangent", ShaderDataType::Float3 },
		{ "aMaterialNumber", ShaderDataType::Int }
	};
};

struct MaterialInstance {
	VertexContainer container;
	uint materialNumber;

	MaterialInstance(const std::vector<VertexImpl>& vertices, uint materialNumber) : container(vertices), materialNumber(materialNumber) {}

	void* data() {
		return container.data();
	}
	size_t size() {
		return container.size();
	}
	size_t stride() {
		return container.stride();
	}
};
