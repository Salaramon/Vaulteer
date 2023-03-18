#pragma once

#include <array>
#include <map>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Mesh.h"
#include "Model/Textures/Texture.h"
#include "Model/Material.h"


class MeshLibrary {
public:
	// returns pointer to material, inserted or existing; material will have library index set
	static Mesh* create(aiMesh* aiMesh, const std::string& objPath) {
		Vertices vertices;
		Indices indices;
		std::vector<Texture> textures;

		for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
			Vertex vertex;
			vertex.aPos = ai_glmVec(aiMesh->mVertices[i]);
			vertex.aNormal = aiMesh->HasNormals() ? ai_glmVec(aiMesh->mNormals[i]) : glm::vec3(0);
			vertex.aTexCoords = aiMesh->HasTextureCoords(0) ? glm::vec2(aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);

			if (aiMesh->HasTangentsAndBitangents()) {
				vertex.aTangent = ai_glmVec(aiMesh->mTangents[i]);
				vertex.aBitangent = ai_glmVec(aiMesh->mBitangents[i]);
			}

			vertices.push_back(vertex);
		}


		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		const auto& mesh = meshByName.emplace(objPath, std::make_unique<Mesh>(vertices, indices));
		meshKeysByIndex[meshByName.size()] = objPath;
		return mesh.first->second.get();
	}

	static Mesh* get(unsigned int index) {
		auto& name = meshKeysByIndex[index];
		return meshByName.at(name).get();
	}

	static Mesh* get(const std::string& name) {
		return meshByName.at(name).get();
	}

	static const std::map<std::string, std::unique_ptr<Mesh>>& getAllMaterials() {
		return meshByName;
	}

	static size_t size() {
		return numMaterials;
	}

private:
	static glm::vec3 ai_glmVec(aiVector3D aiVec) {
		return { aiVec.x, aiVec.y, aiVec.z };
	}

	inline static std::vector<std::string> meshKeysByIndex;
	inline static std::map<std::string, std::unique_ptr<Mesh>> meshByName;

	inline static size_t numMaterials = 0;
	
	// this class is static only
	MeshLibrary() = default;
};
