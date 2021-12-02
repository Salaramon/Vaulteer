#pragma once

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <type_traits>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicsData.h"

#include "Mesh.h"
#include "Texture.h"
#include "VertexHash.h"
#include "MathUtils.h"

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
	const std::vector<Mesh>& getMeshes();
	const std::vector<Texture>& getTextures();

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

	//Outer array is for each axis (xyz => [0][1][2]). Inner array is for min[0] and max[1] extrimity
	std::array<std::array<std::vector<glm::vec3>, 2>, 3> extremities;


	template<size_t dimension, class Store>
	struct _darray {
	public:
		using type = std::array<typename _darray<dimension - 1, Store>::type, dimension * 2>;
	};

	template<class Store>
	struct _darray<0, Store> {
	public:
		using type = Store;
	};

	//template<size_t dimension, class Store>
	//struct darray : public std::array<typename _darray<dimension - 1, Store>::type, dimension * 2> {};

	template<size_t dimension, class Store>
	using darray = std::array<typename _darray<dimension - 1, Store>::type, dimension * 2>;

	//n is the dimensions which will be considered, m is the dimensions of the points
	//m must be greater or equal to n
	template<size_t n, size_t m>
	void processConvexShape(
		const std::vector<std::array<float, m>>& inVertices,
		std::vector<std::array<float, m>>& outVertices,
		std::vector<size_t>& outIndices)
	{
		darray<m, std::array<float, m>> cubeTree;
		//std::unordered_set<size_t> emptySet = std::unordered_set<size_t>();
		_processConvexShape<n, m> func; 
		func(inVertices, cubeTree);
	}



	//n is the dimensions which will be considered, m is the dimensions of the points
	//m must be greater or equal to n
	template<size_t n, size_t m>
	struct _processConvexShape {
	public:
		void operator()(
			const std::vector<std::array<float, m>>& inVertices,
			darray<n, std::array<float, m>>& cubeTree,
			std::unordered_set<size_t> ignoredAxis = std::unordered_set<size_t>()
			) {
			typedef std::function<bool(float, float)> CompOpFunction;
			std::array<CompOpFunction, 2> comparator = { std::less<float>(), std::greater<float>() };

			std::array<std::array<std::vector<std::array<float, m>>, 2>, n> extremities;

			for (auto& v : inVertices) {
				for (size_t i = 0; i < extremities.size(); i++) {
					//Checks if the previous iteration already did that axis or not.
					if (ignoredAxis.find(i) == ignoredAxis.end()) {
						for (size_t j = 0; j < extremities[i].size(); j++) {
							if (!extremities[i][j].empty() || comparator[j](extremities[i][j].back()[i], v[i])) {
								extremities[i][j].clear();
							}
							extremities[i][j].push_back(v);
						}
					}
				}
			}

			for (size_t i = 0; i < extremities.size(); i++) {
				for (size_t j = 0; j < extremities[i].size(); j++) {
					if (n != 1) {
						ignoredAxis.insert(i);
						darray<n - 1, std::array<float, m>>& cubePart = cubeTree[i * 2 + j];
						_processConvexShape<n-1, m> func;
						func(extremities[i][j], cubePart, ignoredAxis);
					}
					else if (!extremities[i][j].empty()) {
						auto& ct = *(std::array<float, m>*)(&cubeTree[j]);
						ct = extremities[i][j].back();
					}
				}
			}
		}

		//Points may be counted twice, ex in the case where one face only have two points
	};

	template<size_t m>
	struct _processConvexShape<0, m> {};
};