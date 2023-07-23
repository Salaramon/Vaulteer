#pragma once
#include <memory>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "MeshLoader.h"
#include "Model/ModelResourceLocator.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Model/Resources/GeometryLibrary.h"

constexpr int blank_import_flags = -1;

class ResourceLoader {

	std::vector<aiTextureType> renderable_texture_types = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	};

public:
	static std::vector<Mesh*> importModel(const ModelResourceLocator& loc) {
		return importModel(loc.path, loc.useAdjacency, loc.smoothNormals);
	}

	static std::vector<Mesh*> importModel(const std::string& objPath, bool useAdjacency = true, bool smoothNormals = false) {
		// default flags
		int importFlags = smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals;
		//assimp optimizations - not sure if these help yet
		importFlags |= aiProcess_JoinIdenticalVertices
					| aiProcess_Triangulate
					| aiProcess_CalcTangentSpace
					| aiProcessPreset_TargetRealtime_Fast & ~aiProcess_GenNormals;

		Assimp::Importer modelImporter;
		const aiScene* scene = modelImporter.ReadFile(objPath, importFlags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			KYSE_ASSERT(false, "Assimp error: " + std::string(modelImporter.GetErrorString()) + "\n")
		}

		// aggregate scene materials, used to populate mesh vertices with material index data
		std::vector<Material*> sceneMaterials;
		for (uint i = 0; i < scene->mNumMaterials; i++) {
			aiMaterial* aiMaterial = scene->mMaterials[i];
			sceneMaterials.push_back(MaterialLibrary::create(aiMaterial, objPath));
		}

		std::vector<Mesh*> meshes;
		meshes.reserve(scene->mNumMeshes);

		MeshSceneParams params = { objPath, scene, sceneMaterials, useAdjacency };
		MeshLoader::loadFromScene<VertexImpl>(meshes, params);

		if (!meshes.empty())
			numModels++;

		return meshes;
	}

private:
	inline static int numModels = 0;

	// this class is static only
	ResourceLoader() = default;
};