#pragma once
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Mesh.h"
#include "Model/Storage/VertexImpl.h"
#include "Model/Textures/Texture.h"

class MeshLoader {
public:
	// creates Mesh set from scene populated with MaterialVertex data and material
	// could be templated in the future based on vertex type

	static void loadFromScene(std::vector<std::unique_ptr<Mesh>>& meshes, const aiScene* scene, std::vector<Material*>& sceneMaterials) {
		processNode(meshes, scene, scene->mRootNode, sceneMaterials);
	}

	static std::unique_ptr<Mesh> loadFromMesh(const aiMesh* aiMesh, Material* mat) {
		return processMesh<VertexImpl>(aiMesh, mat);
	}

private:
	static void processNode(std::vector<std::unique_ptr<Mesh>>& meshes, const aiScene* scene, const aiNode* node, std::vector<Material*>& sceneMaterials) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			auto mat = sceneMaterials.at(aiMesh->mMaterialIndex);

			auto& mesh = meshes.emplace_back(processMesh<VertexImpl>(aiMesh, mat));
			mesh->useAdjacency();
		}

		for (size_t i = 0; i < node->mNumChildren; i++) {
			processNode(meshes, scene, node->mChildren[i], sceneMaterials);
		}
	}

	template <vertex_concept T>
	static std::unique_ptr<Mesh> processMesh(const aiMesh* aiMesh, Material* mat) {
		
		std::vector<T> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		GLuint numFaces = aiMesh->mNumFaces;

		vertices.reserve(aiMesh->mNumVertices);
		indices.reserve(aiMesh->mNumFaces);
		textures.reserve(Material::validTextureTypes.size());

		for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
			T vertex = T::generateVertex(aiMesh, i);
			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
		
		return std::make_unique<Mesh>( vertices, indices, numFaces, mat );
	}

};
