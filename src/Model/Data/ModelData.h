#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <type_traits>

#include <assimp/scene.h>

#include <glm/glm.hpp>

#include "Model/Textures/Texture2DArray.h"
#include "Model/Storage/VertexHash.h"
#include "Model/Mesh.h"


class ModelData {
public:
	struct ModelVertexHash : VertexHash<Vertex> {
		size_t hash_combine(size_t lhs, size_t rhs) const {
			lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
			return lhs;
		}

		bool operator()(const Vertex& a, const Vertex& b) const override {
			return a.aPos.x == b.aPos.x && a.aPos.y == b.aPos.y && a.aPos.z == b.aPos.z;
		}

		size_t operator()(const Vertex& k) const override {
			return hash_combine(
				hash_combine(std::hash<float>()(k.aPos.x), std::hash<float>()(k.aPos.y)),
				std::hash<float>()(k.aPos.z));
		}
	};

	struct ModelUnitData {
		int xDelta, yDelta, wDelta, hDelta, layerDelta;
		ModelUnitData() : xDelta(0), yDelta(0), wDelta(0), hDelta(0), layerDelta(1) {}
		ModelUnitData(const Texture2DArray::TextureUnit& u) : xDelta(u.x), yDelta(u.y), wDelta(u.w), hDelta(u.h), layerDelta(u.layer) {}
	};

	struct ModelUnitTable {
		ModelUnitData diffuseUnit, specularUnit, normalMapUnit;

		ModelUnitTable() = default;

		void setUnit(aiTextureType ttype, const Texture2DArray::TextureUnit& u) {
			switch (ttype) {
			case aiTextureType_DIFFUSE: diffuseUnit = u;
				break;
			case aiTextureType_SPECULAR: specularUnit = u;
				break;
			case aiTextureType_HEIGHT: normalMapUnit = u;
				break;
			default: ;
			}
		}

		const ModelUnitData& operator[](size_t index) const {
			switch (index) {
			case 0: return diffuseUnit;
			case 1: return specularUnit;
			case 2: return normalMapUnit;
			default: throw std::exception("invalid index");
			}
		}
	};

	glm::vec4 getBoundingSphere();

	//ModelData(GLsizei width, GLsizei height, std::vector<glm::vec4> colors, std::vector<Vertex> vertices);
	ModelData(const std::string& modelPath, std::vector<Mesh>& meshVec);
	ModelData(ModelData&& other) noexcept;

	std::vector<Mesh>& getMeshes();
	GLint getTextureID() const;
	unsigned int getMaterialIndex() const;
	const ModelUnitTable& getModelUnitTable() const;

	void updateWithTextureUnits(const Texture2DArray& texture);

private:
	std::string modelPath;
	GLint textureID{};

	// populated on load (loadModel)
	std::vector<Mesh> meshes;

	// populated after texture packing (updateWithTextureUnits)
	std::unordered_map<std::string, Texture2DArray::TextureUnit> unitByTexturePath;
	ModelUnitTable modelUnitTable;
	unsigned int materialIndex = 0;

	std::unordered_set<std::string> textureFiles;


	/*

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
		ProcessConvexShape<n, m>::getPointExtremities(inVertices, cubeTree);
		
		ProcessConvexShape<n, m>::getConvexCubeTreePointsAndIndices(cubeTree, outVertices, outIndices);
	}

	struct shared_ProcessConvexShape {
	public:

	protected:
		typedef std::function<bool(float, float)> CompOpFunction;
		inline static const std::array<CompOpFunction, 2> comparator = { std::less<float>(), std::greater<float>() };

		template<size_t n, size_t m>
		static void findExtremities(
			const std::vector<std::array<float, m>>& inPoints,
			std::array<std::array<std::vector<std::array<float, m>>, 2>, m>& outExtremities,
			std::unordered_set<size_t> ignoredAxis
		) {
			for (const std::array<float, m>& v : inPoints) {
				for (size_t i = 0; i < outExtremities.size(); i++) {
					//Checks if the previous iteration already did that axis or not.
					if (ignoredAxis.find(i) == ignoredAxis.end()) {
						for (size_t j = 0; j < outExtremities[i].size(); j++) {
							if (!outExtremities[i][j].empty()) {
								bool comp = comparator[j](v[i], outExtremities[i][j].back()[i]);
								if (comp) {
									outExtremities[i][j].clear();
									outExtremities[i][j].push_back(v);
								}
								else if (outExtremities[i][j].back()[i] == v[i]) {
									outExtremities[i][j].push_back(v);
								}
							}
							else {
								outExtremities[i][j].push_back(v);
							}
						}
					}
				}
			}
		}

		template<size_t n, size_t m>
		static void processExtremitySubsets(
			std::array<std::array<std::vector<std::array<float, m>>, 2>, m>& outExtremities,
			darray<n, std::array<float, m>>& cubeTree,
			std::unordered_set<size_t> ignoredAxis
		) {
			size_t abc = cubeTree.size();
			size_t cubeTreeCount = 0;
			for (size_t i = 0; i < outExtremities.size(); i++) {
				std::unordered_set<size_t> ignoredAxisPass = ignoredAxis;
				for (size_t j = 0; j < outExtremities[i].size(); j++) {
					if (ignoredAxis.find(i) == ignoredAxis.end()) {
						ignoredAxisPass.insert(i);
						//darray<n - 1, std::array<float, m>>& cubePart = cubeTree[cubeTreeCount * 2 + j];
						
						ProcessConvexShape<n - 1, m>::getPointExtremities(outExtremities[i][j], cubeTree[cubeTreeCount * 2 + j], ignoredAxisPass);
					}
				}
				if (ignoredAxis.find(i) == ignoredAxis.end()) {
					cubeTreeCount++;
				}
			}
		}

		template<size_t n, size_t m>
		static void processExtremityEnds(
			std::array<std::array<std::vector<std::array<float, m>>, 2>, m>& outExtremities,
			darray<n, std::array<float, m>>& cubeTree,
			std::unordered_set<size_t> ignoredAxis
		) {
			for (size_t i = 0; i < outExtremities.size(); i++) {
				for (size_t j = 0; j < outExtremities[i].size(); j++) {
					if (!outExtremities[i][j].empty()) {
						cubeTree[j] = outExtremities[i][j].back();
					}
				}
			}
		}
	};

	//n is the dimensions which will be considered, m is the dimensions of the points
	//m must be greater or equal to n
	template<size_t n, size_t m>
	struct ProcessConvexShape : public shared_ProcessConvexShape {
	public:
		static void getPointExtremities(
			const std::vector<std::array<float, m>>& inPoints,
			darray<n, std::array<float, m>>& cubeTree,
			std::unordered_set<size_t> ignoredAxis = std::unordered_set<size_t>()
		) {
			std::array<std::array<std::vector<std::array<float, m>>, 2>, m> extremities;

			findExtremities<n, m>(inPoints, extremities, ignoredAxis);
			processExtremitySubsets<n, m>(extremities, cubeTree, ignoredAxis);
			
		}

		static void getConvexCubeTreePointsAndIndices(
			const darray<n, std::array<float, m>>& cubeTree, 
			std::vector<std::array<float, m>>& outPoints, 
			std::vector<size_t>& outIndices
		) {
			for (size_t i = 0; i < cubeTree.size(); i++) {
				ProcessConvexShape<n - 1, m>::getConvexCubeTreePointsAndIndices(cubeTree[i], outPoints, outIndices);

			}
		}

	private:

		//Points may be counted twice, ex in the case where one face only have two points
	};

	template<size_t m>
	struct ProcessConvexShape<1, m> : public shared_ProcessConvexShape {
		static void getPointExtremities(
			const std::vector<std::array<float, m>>& inPoints,
			darray<1, std::array<float, m>>& cubeTree,
			std::unordered_set<size_t> ignoredAxis = std::unordered_set<size_t>()
		) {
			std::array<std::array<std::vector<std::array<float, m>>, 2>, m> extremities;

			findExtremities<1,m>(inPoints, extremities, ignoredAxis);
			processExtremityEnds<1, m>(extremities, cubeTree, ignoredAxis);
		}

		static void getConvexCubeTreePointsAndIndices(
			const darray<1, std::array<float, m>>& cubeTree, 
			std::vector<std::array<float, m>>& outVertices, 
			std::vector<size_t>& outIndices
		) {
			for (size_t i = 0; i < cubeTree.size(); i++) {
				outVertices.push_back(cubeTree[i]);
			}
		}
			
	};
	*/
};
