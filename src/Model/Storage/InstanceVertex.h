#pragma once

#include "Vertex.h"
#include "BufferLayout.h"

#include "GLSLCPPBinder.h"

class InstanceVertex : Vertex {
public:
	InstanceVertex(glm::mat4 instanceMat = glm::mat4(1.0)) : InstanceVertex(glm::vec3(0.0), instanceMat) {}

	InstanceVertex(glm::vec3 point, glm::mat4 instanceMat = glm::mat4(1.0)) : aInstanceMat(instanceMat) {}

	glm::mat4 aInstanceMat;

	static BufferLayout& getFormat() {
		return format;
	}

private:
	inline static BufferLayout format = {
		{"aInstanceMat", ShaderDataType::Mat4 }
	};
};
