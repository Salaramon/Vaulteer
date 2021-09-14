#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Texture.h"

#include "DebugLogger.h"

class ModelData : public DebugLogger<ModelData>
{
public:
	ModelData(std::string meshPath);
	ModelData(std::string meshPath, std::string textureFolderPath);

	void loadModel(std::string path);

	void draw(const Shader& shader);

private:

	void processNode(const aiScene* scene, aiNode* node);

	Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	void getTextureUniforms(aiMaterial* material);

	glm::vec3 ai_glmVec(aiVector3D aiVec);

	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
	std::string texturesFolder;

	void setTexturesFolder(std::string path);

	std::unordered_set<std::string>  textureFiles;
};

