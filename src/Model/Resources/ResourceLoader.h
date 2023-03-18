#pragma once
#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/ModelResourceLocator.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Model/Resources/MeshLibrary.h"

constexpr int blank_import_flags = -1;

class ResourceLoader {

	std::vector<aiTextureType> renderable_texture_types = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	};

public:
	static std::vector<Mesh*> importModel(const ModelResourceLocator& loc) {
		return importModel(loc.path, loc.importFlags);
	}

	static std::vector<Mesh*> importModel(const std::string& objPath, int importFlags = blank_import_flags) {
		std::vector<Material*> sceneMaterials;
		std::vector<Mesh*> meshes;

		// default flags
		if (importFlags == blank_import_flags)
			importFlags = aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

		Assimp::Importer modelImporter;
		const aiScene* scene = modelImporter.ReadFile(objPath, importFlags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			// TODO: still need logging in static context
			std::cout << "Assimp Error: " + std::string(modelImporter.GetErrorString()) + "\n" << std::endl;
			assert(false);
		}


		for (int i = 0; i < scene->mNumMaterials; i++) {
			aiMaterial* aiMaterial = scene->mMaterials[i];
			sceneMaterials.emplace_back(MaterialLibrary::create(aiMaterial, objPath));
		}

		// maybe we want to use a MeshLibrary at some point so ModelData doesn't own its meshes; it can be used here

		if (scene) {
			processNode(meshes, sceneMaterials, scene, scene->mRootNode, objPath);
		}

		numModels++;

		return meshes;
	};

private:
	//void processNode(std::vector<Mesh>& meshes, std::vector<Material>& materials, const aiScene* scene, aiNode* node);
	static void processNode(std::vector<Mesh*>& meshes, std::vector<Material*>& sceneMaterials, const aiScene* scene, const aiNode* node, const std::string& objPath) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

			meshes.emplace_back(MeshLibrary::create(aiMesh, objPath));
		}

		for (size_t i = 0; i < node->mNumChildren; i++) {
			processNode(meshes, sceneMaterials, scene, node->mChildren[i], objPath);
		}
	}


	inline static int numModels = 0;

	// this class is static only
	ResourceLoader() = default;

	
};