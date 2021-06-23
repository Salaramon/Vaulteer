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
	void rotate(float angle, glm::vec3 axis);
	void setRotation(float angle, glm::vec3 axis);
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);
	void draw();
private:


	glm::mat4 modelScale = glm::mat4(1.0f);
	glm::mat4 modelTranslation = glm::mat4(1.0f);
	glm::mat4 modelRotation = glm::mat4(1.0f);

	void loadModel(std::string path);

	void loadTexture(const Uniform uniform, const std::string path);

	void processNode(const aiScene* scene, aiNode* node);

	Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	void getTextureUniforms(aiMaterial* material);

	glm::vec3 ai_glmVec(aiVector3D aiVec);

	Textures textures;
	std::vector<Mesh> meshes;
	std::string texturesFolder;

	Shader* shader;

	void setTexturesFolder(std::string path);

	std::unordered_set<std::string>  textureFiles;

};

