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
#include "Technique.h"

#include "DebugLogger.h"

typedef std::vector<Texture> Textures;

class Model : public DebugLogger<Model>
{
public:
	Model(std::string meshPath, const size_t instances = 1);
	Model(std::string meshPath, std::string textureFolderPath, const size_t instances = 1);

	Model& operator[](size_t instance);

	void setShaderContext(Technique* technique);
	void rotate(float angle, glm::vec3 axis);
	void setRotation(float angle, glm::vec3 axis);
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);
	void draw(Technique& technique);
private:


	std::vector<glm::mat4> modelScale = {};
	std::vector<glm::mat4> modelTranslation = {};
	std::vector<glm::mat4> modelRotation = {};

	void loadModel(std::string path);

	void loadTexture(const Binder::UniformInfo uniform, const std::string path);

	void processNode(const aiScene* scene, aiNode* node);

	Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	void getTextureUniforms(aiMaterial* material);

	glm::vec3 ai_glmVec(aiVector3D aiVec);

	Textures textures;
	std::vector<Mesh> meshes;
	std::string texturesFolder;

	Technique* technique;

	void setTexturesFolder(std::string path);

	std::unordered_set<std::string>  textureFiles;

	size_t instanceSelection = 0;
	std::vector<glm::mat4> instances;
	VertexBuffer<glm::mat4> instanceBuffer;

};

