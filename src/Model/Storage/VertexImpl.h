#pragma once

#include <glm/glm.hpp>
#include <assimp/mesh.h>

#include "Vertex.h"
#include "VertexBufferLayout.h"
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
	
	static VertexImpl generateVertex(const aiMesh* aiMesh, size_t i) {
		VertexImpl vertex;
		vertex.aPos = ai_glmVec(aiMesh->mVertices[i]);
		vertex.aNormal = aiMesh->HasNormals() ? ai_glmVec(aiMesh->mNormals[i]) : glm::vec3(0);
		vertex.aTexCoords = aiMesh->HasTextureCoords(0) ? glm::vec2(aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);

		if (aiMesh->HasTangentsAndBitangents()) {
			vertex.aTangent = ai_glmVec(aiMesh->mTangents[i]);
			vertex.aBitangent = ai_glmVec(aiMesh->mBitangents[i]);
		}

		return vertex;
	}

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
		{ "aMaterialNumber", ShaderDataType::Int, INT32_MAX }, // instanced
		{ "instance", ShaderDataType::Mat4, 1} // instanced
	};
};
