#pragma once
#include <memory>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "MeshLoader.h"
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
		// default flags
		if (importFlags == blank_import_flags)
			importFlags = aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

		//assimp optimizations - not sure if these help yet
		importFlags |= aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_Fast & ~aiProcess_GenNormals;

		Assimp::Importer modelImporter;
		const aiScene* scene = modelImporter.ReadFile(objPath, importFlags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "Assimp Error: " + std::string(modelImporter.GetErrorString()) + "\n" << std::endl;
			assert(false);
		}

		// aggregate scene materials, used to populate mesh vertices with material index data
		std::vector<Material*> sceneMaterials;
		for (uint i = 0; i < scene->mNumMaterials; i++) {
			aiMaterial* aiMaterial = scene->mMaterials[i];
			sceneMaterials.push_back(MaterialLibrary::create(aiMaterial, objPath));
		}

		auto meshes = MeshLibrary::create(objPath, scene, sceneMaterials);
		if (!meshes.empty())
			numModels++;

		return meshes;
	}

private:
	inline static int numModels = 0;

	// this class is static only
	ResourceLoader() = default;
};