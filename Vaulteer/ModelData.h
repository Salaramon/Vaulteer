#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicsData.h"

#include "Mesh.h"
#include "Texture2DArray.h"
#include "VertexHash.h"

#include "DebugLogger.h"

class ModelData : public DebugLogger<ModelData>, public GraphicsData
{
public:

	struct ModelVertexHash : public VertexHash<Vertex> {
	public:
		size_t hash_combine(size_t lhs, size_t rhs) const {
			lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
			return lhs;
		}

		bool operator()(const Vertex& a, const Vertex& b)const override
		{
			return a.aPos.x == b.aPos.x && a.aPos.y == b.aPos.y && a.aPos.z == b.aPos.z;
		}

		size_t operator()(const Vertex& k) const override
		{
			return hash_combine(
				hash_combine(std::hash<float>()(k.aPos.x), std::hash<float>()(k.aPos.y)),
				std::hash<float>()(k.aPos.z));
		}
	};

	ModelData(GLsizei width, GLsizei height, std::vector<glm::vec4> colors, std::vector<Vertex> vertices);
	ModelData(std::string meshPath);
	ModelData(std::string meshPath, std::string textureFolderPath);

	void loadModel(std::string path);

	void draw(const Shader& shader) override;
	const Texture2DArray& getTextureArray();
	const std::vector<Mesh>& getMeshes();

private:

	void processNode(const aiScene* scene, aiNode* node);

	Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	void getTextureUniforms(aiMaterial* material);

	glm::vec3 ai_glmVec(aiVector3D aiVec);

	//std::vector<std::pair<aiTextureType, Texture>> textures;
	std::unique_ptr<Texture2DArray> textureArray;
	std::vector<Mesh> meshes;

	void setTexturesFolder(std::string path);

	std::string texturesFolder;
	std::vector<std::string> textureFiles;
};