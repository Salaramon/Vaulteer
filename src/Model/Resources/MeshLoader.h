#pragma once
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Mesh.h"
#include "Model/Storage/VertexImpl.h"
#include "Model/Textures/Texture.h"
#include "Model/Resources/GeometryLibrary.h"


struct MeshSceneParams {
	const std::string& objPath;
	const aiScene* scene;
	const std::vector<Material*>& materials;
	bool useAdjacency;
};


class MeshLoader {
public:
	// creates Mesh set from scene populated with vertex data and material

	template <vertex_concept T>
	static void loadFromScene(std::vector<Mesh*>& meshes, const MeshSceneParams& params) {
		processNode<T>(meshes, params, params.scene->mRootNode, 0);
	}

	template <vertex_concept T>
	static void processNode(std::vector<Mesh*>& meshes, const MeshSceneParams& params, aiNode* node, int meshCounter) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aiMesh = params.scene->mMeshes[node->mMeshes[i]];
			auto mat = params.materials.at(aiMesh->mMaterialIndex);

			// todo identifier for geometry might not be needed
			std::string geometryName = params.objPath;
			geometryName.append("_").append(std::to_string(meshCounter));

			auto& mesh = meshes.emplace_back(processMesh<T>(geometryName, aiMesh, mat, params.useAdjacency));
			meshCounter++;
		}

		for (size_t i = 0; i < node->mNumChildren; i++) {
			processNode<T>(meshes, params, node->mChildren[i], meshCounter);
		}
	}
	
	template <vertex_concept T>
	static Mesh* processMesh(const std::string& name, const aiMesh* aiMesh, Material* mat, bool useAdjacency) {
		Geometry* geometry = processGeometry<T>(name, aiMesh, useAdjacency);

		// todo: funny allocation, use internal memory later
		return new Mesh(geometry, mat);
	}

	template <vertex_concept T>
	static Geometry* processGeometry(const std::string& name, const aiMesh* aiMesh, bool useAdjacency) {
		Geometry* existing = GeometryLibrary::get(name);
		//KYSE_ASSERT(existing == nullptr)

		if (existing != nullptr)
			return existing;
		
		std::vector<T> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		vertices.reserve(aiMesh->mNumVertices);
		indices.reserve(aiMesh->mNumFaces * 3);
		textures.reserve(Material::validTextureTypes.size());

		GLenum type = useAdjacency ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;

		for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
			T vertex = T::generateVertex(aiMesh, i);
			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		auto geometry = GeometryLibrary::create( name, vertices, indices, type );
		if (useAdjacency) {
			genAdjacencyInfo<T>(geometry, aiMesh->mNumFaces);
		}
		Log::trace("Loaded geometry{} with name: {}", (useAdjacency ? " with adjacency" : ""), name);
		return geometry;
	}


	// -- Geometry adjacency calculation --
	// TODO this should use half edges instead, probably

	struct VectorComp {
		bool operator()(const glm::vec3& a, const glm::vec3& b) const {
			if (a.x < b.x) {
				return true;
			}
			if (a.x == b.x) {
				if (a.y < b.y) {
					return true;
				}
				if (a.y == b.y) {
					if (a.z < b.z) {
						return true;
					}
				}
			}
			return false;
		}
	};
	typedef std::map<glm::vec3, GLuint, VectorComp> position_index_map;

	
	template <vertex_concept T>
	static void genAdjacencyInfo(Geometry* geometry, size_t numFaces) {
		auto& vertices = geometry->vertices;
		auto& indices = geometry->indices;

		std::vector<GLuint> newIndices(numFaces * 6);
		position_index_map posIndexMap; // maps one unique index for every vertex position vector

		// Generate map that maps each vertex position to a unique index
		genUniqueIndexMap<T>(vertices, indices, posIndexMap, numFaces);

		for (int i = 0; i < numFaces; i++) {
			GLuint firstIdx = 3 * i; // The first index of the triangle in the index array
			GLuint newFirstIdx = 6 * i;

			// Get unique index for each vertex in the triangle
			GLuint v[3];
			for (int j = 0; j < 3; j++) {
				GLuint idx = indices[firstIdx + j];
				v[j] = posIndexMap[vertices.at<T>(idx)->aPos];
			}

			// Create new indexes to be used for adjacency mode
			for (int j = 0; j < 3; j++) {
				// Original vertice stays the same 
				newIndices[newFirstIdx + 2 * j] = indices[firstIdx + j];

				// Find and add neighboring vertice to list
				GLuint n = findAdjacentVertexIdx<T>(vertices, indices, posIndexMap, numFaces, v[j], v[(j + 1) % 3], v[(j + 2) % 3]);
				newIndices[newFirstIdx + 2 * j + 1] = n;
			}
		}
		
		for (GLuint i = 0; i < newIndices.size(); i += 6) {
			if (newIndices[i + 1] == -1) newIndices[i + 1] = newIndices[i + 4];
			if (newIndices[i + 3] == -1) newIndices[i + 3] = newIndices[i];
			if (newIndices[i + 5] == -1) newIndices[i + 5] = newIndices[i + 2];
		}

		geometry->indices = newIndices;
	}
	
	template <vertex_concept T>
	static void genUniqueIndexMap(VertexContainer& vertices, std::vector<GLuint> indices, position_index_map& posIndexMap, size_t numFaces) {
		for (int i = 0; i < numFaces; i++) {
			int firstIdx = 3 * i; // The first index of the triangle in the index array

			// For each vertex position, check if in the map.
			// If not, add it and the current index. 
			for (int j = 0; j < 3; j++) {
				int idx = indices[firstIdx + j];
				glm::vec3 pos = vertices.at<T>(idx)->aPos;

				if (!posIndexMap.contains(pos)) {
					posIndexMap[pos] = idx;
				}
			}
		}
	}
	
	template <vertex_concept T>
	static int findAdjacentVertexIdx(VertexContainer& vertices, std::vector<GLuint> indices, position_index_map& posIndexMap, 
							  size_t numFaces, GLuint startIdx, GLuint endIdx, GLuint oppIdx) {
		for (int i = 0; i < numFaces; i++) {
			int firstIdx = 3 * i;

			// Get the unique indices for the vertices
			GLuint faceIndices[3];
			for (int j = 0; j < 3; j++) {
				GLuint idx = indices[firstIdx + j];
				faceIndices[j] = posIndexMap[vertices.at<T>(idx)->aPos];
			}

			// For each edge in the triangle, check its opposite vertice
			for (int edge = 0; edge < 3; edge++) {
				GLuint v0 = faceIndices[edge];
				GLuint v1 = faceIndices[(edge + 1) % 3];
				GLuint v2 = faceIndices[(edge + 2) % 3]; // Opposite index to side being tested

				if ((v0 == startIdx && v1 == endIdx) || (v1 == startIdx && v0 == endIdx))
				{
					// If last index is not the opposite of the edge we are comparing with
					// => we have found the adjacent vertex.
					if (v2 != oppIdx) return v2;
				}
			}
		}

		return -1; // no neighbor found
	}

};
