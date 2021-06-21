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
#include "Shader.h"

typedef std::vector<Texture> Textures;

class Model
{
public:
	Model(std::string meshPath);
	Model(std::string meshPath, std::string textureFolderPath);

	void setShaderContext(Shader* shader);
	void draw();
private:

	void loadModel(std::string path);

	void loadTexture(const Uniform uniform, const std::string path);

	void processNode(const aiScene* scene, aiNode* node);

	Mesh&& processMesh(const aiScene* scene, aiMesh* mesh);

	void getTextureUniforms(aiMaterial* material);

	glm::vec3 ai_glmVec(aiVector3D aiVec);

	//void renderingLogic() override;

	Textures textures;
	std::vector<Mesh> meshes;
	std::string texturesFolder;

	Shader* shader;

	void setTexturesFolder(std::string path);

	//std::unordered_map<std::string, size_t>  uniformUsage;
	std::unordered_set<std::string>  textureFiles;

};

