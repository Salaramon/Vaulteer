#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Data/ModelData.h"

class ResourceLoader {
	std::vector<aiTextureType> renderable_texture_types = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	};

public:
	static ModelData importModel(const std::string& objPath, int importFlags = -1);

	static std::unordered_map<std::string, Material>& getMaterialLibrary();

private:
	//void processNode(std::vector<Mesh>& meshes, std::vector<Material>& materials, const aiScene* scene, aiNode* node);
	static void processNode(std::vector<Mesh>& meshes, const aiScene* scene, aiNode* node);

	//Mesh processMesh(std::vector<Material>& materials, const aiScene* scene, aiMesh* mesh);
	static Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	static glm::vec3 ai_glmVec(aiVector3D aiVec);


	inline static std::vector<std::string> materialKeysByIndex;
	inline static std::unordered_map<std::string, Material> materialLibrary;

	inline static size_t numMaterials = 0;
	inline static int numModels = 0;
};
