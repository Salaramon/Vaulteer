#pragma once

#include "Debug/Debug.h"
#include "Vertex.h"
#include "BufferLayout.h"

#include "GLSLCPPBinder.h"

class InstanceVertex : Vertex {
public:
	InstanceVertex(glm::mat4 instanceMat = glm::mat4(1.0)) : InstanceVertex(glm::vec3(0.0), instanceMat) {
		LOG::CTOR::debug(this, "Vertex constructor called");
	}

	InstanceVertex(glm::vec3 point, glm::mat4 instanceMat = glm::mat4(1.0)) : aPos(point), aNormal(0.0), aTexCoords(0.0), aTangent(0.0), aBitangent(0.0), aMaterialNumber(0),
			aInstanceMat(instanceMat) {
		LOG::CTOR::debug(this, "Vertex constructor called");
	}

	glm::vec3 aPos;
	glm::vec3 aNormal;
	glm::vec2 aTexCoords;
	glm::vec3 aTangent;
	glm::vec3 aBitangent;
	unsigned int aMaterialNumber;
	glm::mat4 aInstanceMat;

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
		{"aMaterialNumber", ShaderDataType::Int },
		{"aInstanceMat", ShaderDataType::Mat4 }
	};


	DY::ObjectRegister<InstanceVertex> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<DY::No_CB, decltype(OB), DY::No_FB, DY::No_VB>;
};
