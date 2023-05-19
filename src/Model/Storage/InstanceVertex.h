#pragma once

#include "Vertex.h"
#include "BufferLayout.h"

#include <glm/glm.hpp>

class InstanceVertex : Vertex {
public:
	InstanceVertex(glm::mat4 instanceMat = glm::mat4(1.0)) : aInstanceMat(instanceMat) {}

	glm::mat4 aInstanceMat;

	static BufferLayout& getFormat() {
		return format;
	}

private:
	inline static BufferLayout format = {
		{"aInstanceMat", ShaderDataType::Mat4 }
	};
};
