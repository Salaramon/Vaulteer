#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/ModelResourceLocator.h"
#include "Model/Data/ModelData.h"
#include "Model/Resources/MaterialLibrary.h"

constexpr int blank_import_flags = -1;

class ResourceLoader {
	std::vector<aiTextureType> renderable_texture_types = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	};

public:
	static std::unique_ptr<ModelData> importModel(const ModelResourceLocator& loc);
	static std::unique_ptr<ModelData> importModel(const std::string& objPath, int importFlags = blank_import_flags);

private:
	//void processNode(std::vector<Mesh>& meshes, std::vector<Material>& materials, const aiScene* scene, aiNode* node);
	static void processNode(std::vector<Mesh>& meshes, std::vector<std::shared_ptr<Material>>& sceneMaterials, const aiScene* scene, const aiNode* node);

	//Mesh processMesh(std::vector<Material>& materials, const aiScene* scene, aiMesh* mesh);
	static Mesh processMesh(std::vector<std::shared_ptr<Material>>& sceneMaterials, aiMesh* mesh);

	static glm::vec3 ai_glmVec(aiVector3D aiVec);


	inline static int numModels = 0;

	// this class is static only
	ResourceLoader() = default;
};
