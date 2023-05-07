#pragma once
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Mesh.h"
#include "Model/Storage/MaterialVertex.h"
#include "Model/Textures/Texture.h"

class MeshLoader {
public:
	// creates Mesh set from scene populated with MaterialVertex data and material
	// could be templated in the future based on vertex type

	static void loadFromScene(std::vector<std::unique_ptr<Mesh>>& meshes, const aiScene* scene, std::vector<Material*>& sceneMaterials) {
		processNode(meshes, scene, scene->mRootNode, sceneMaterials);
	}

	static std::unique_ptr<Mesh> loadFromMesh(const aiMesh* aiMesh, Material* mat) {
		return processMesh(aiMesh, mat);
	}

private:
	static void processNode(std::vector<std::unique_ptr<Mesh>>& meshes, const aiScene* scene, const aiNode* node, std::vector<Material*>& sceneMaterials) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			auto mat = sceneMaterials.at(aiMesh->mMaterialIndex);

			meshes.emplace_back(processMesh(aiMesh, mat));
		}

		for (size_t i = 0; i < node->mNumChildren; i++) {
			processNode(meshes, scene, node->mChildren[i], sceneMaterials);
		}
	}
	
	static std::unique_ptr<Mesh> processMesh(const aiMesh* aiMesh, Material* mat) {
		
		std::vector<MaterialVertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
			MaterialVertex vertex = generateVertex(aiMesh, mat, i);
			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
		
		return std::make_unique<Mesh>( vertices, indices, mat );
	}

	static MaterialVertex generateVertex(const aiMesh* aiMesh, const Material* mat, size_t i) {
		MaterialVertex vertex;
		vertex.aPos = ai_glmVec(aiMesh->mVertices[i]);
		vertex.aNormal = aiMesh->HasNormals() ? ai_glmVec(aiMesh->mNormals[i]) : glm::vec3(0);
		vertex.aTexCoords = aiMesh->HasTextureCoords(0) ? glm::vec2(aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);

		if (aiMesh->HasTangentsAndBitangents()) {
			vertex.aTangent = ai_glmVec(aiMesh->mTangents[i]);
			vertex.aBitangent = ai_glmVec(aiMesh->mBitangents[i]);
		}

		vertex.aMaterialNumber = mat->materialIndex;
		return vertex;
	}


	static glm::vec3 ai_glmVec(aiVector3D aiVec) {
		return { aiVec.x, aiVec.y, aiVec.z };
	}
};
