#pragma once

#include <glad/glad.h>
#include <map>

#include "Model/VertexBuffer.h"
#include "Model/IndexBuffer.h"
#include "Model/VertexArray.h"

#include "Model/Material.h"
#include "Storage/VertexImpl.h"


struct VectorComparator {
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

class Mesh {
public:
	VertexContainer vertexContainer;
	std::vector<GLuint> indices;
	GLuint numFaces;
	Material* material;

	// adjacency information used in shadow volume rendering
	bool adjacency = false;
	std::vector<GLuint> adjacencyIndices = {};
	std::map<glm::vec3, GLuint, VectorComparator> posIndexMap; // maps one unique index for every vertex position vector
	IndexBuffer* adjacencyIndexBuffer;

	VertexArray vertexArray;
	std::vector<VertexBuffer*> vertexBuffers;
	IndexBuffer* indexBuffer;

	uint instanceCount = 1;


	template <class T>
	Mesh(std::vector<T> vertices, std::vector<GLuint>& indices, GLuint numFaces, Material* material) :
		vertexContainer(vertices),
		indices(indices),
		numFaces(numFaces),
		material(material) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot create mesh of non-vertices.");

		vertexBuffers = vertexArray.createVertexBuffers(T::getFormat());
		indexBuffer = vertexArray.createIndexBuffer(this->indices);

		insertVertices();
		insertMaterial();

		glm::mat4 ins(1.0);
		insertInstances({ins});
	}

	Mesh(Mesh& other) = delete;

	Mesh(Mesh&& other) noexcept :
		vertexContainer(std::move(other.vertexContainer)),
		indices(std::move(other.indices)),
		material(other.material),
		vertexArray(std::move(other.vertexArray)),
		vertexBuffers(other.vertexBuffers),
		indexBuffer(other.indexBuffer) {}

	~Mesh() = default;

	void bind() {
		vertexArray.bind();
	}

	static void unbind() {
		VertexArray::unbind();
	}

	template <class T>
	std::vector<T> getCopiedData() {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot copy out vertices to non-vertex type.");
		T* arr = (T*) vertexContainer.data();
		return std::vector<T>(arr, arr + vertexContainer.size());
	}

	template <class T>
	T* getVertex(int index) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot copy out vertices to non-vertex type.");
		return vertexContainer.at<T>(index);
	}

	void overrideMaterial(Material* material) {
		this->material = material;
		insertMaterial();
	}


	void insertVertices() {
		vertexBuffers[0]->insert(vertexContainer.data(), vertexContainer.size());
	}

	void insertMaterial() {
		std::vector<int> mats(instanceCount, material->materialIndex);
		vertexBuffers[1]->insert(mats.data(), 1, GL_DYNAMIC_DRAW);
	}

	void insertInstances(const std::vector<glm::mat4>& instanceMats) {
		instanceCount = instanceMats.size();
		vertexBuffers[2]->insert(instanceMats.data(), instanceCount);
	}


	GLenum getType() {
		return adjacency ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
	}

	int getNumIndices() {
		//return indices.size();
		return adjacency ? adjacencyIndices.size() : indices.size();
	}

	bool isInstanced() {
		return instanceCount > 1;
	}

	// TODO this should use half edges instead

	void useAdjacency() {
		if (adjacency)
			return;

		if (adjacencyIndices.empty()) 
			genAdjacencyInfo();
		
		indexBuffer = vertexArray.createIndexBuffer(adjacencyIndices);
	}

	void genAdjacencyInfo() {
		std::vector<GLuint> newIndices(6 * numFaces);

		// Generate map that maps each vertex position to a unique index
		genUniqueIndexMap();

		for (int i = 0; i < numFaces; i++) {
			GLuint firstIdx = 3 * i; // The first index of the triangle in the index array
			GLuint newFirstIdx = 6 * i;

			// Get unique index for each vertex in the triangle
			GLuint v[3];
			for (int j = 0; j < 3; j++) {
				GLuint idx = indices[firstIdx + j];
				v[j] = posIndexMap[vertexContainer.at<VertexImpl>(idx)->aPos];
			}

			// Create new indexes to be used for adjacency mode
			for (int j = 0; j < 3; j++) {
				// Original vertice stays the same 
				newIndices[newFirstIdx + 2 * j] = indices[firstIdx + j];

				// Find and add neighboring vertice to list
				GLuint n = findAdjacentVertexIdx(v[j], v[(j + 1) % 3], v[(j + 2) % 3]);
				newIndices[newFirstIdx + 2 * j + 1] = n;
			}
		}
		
		for (GLuint i = 0; i < newIndices.size(); i += 6) {
			if (newIndices[i + 1] == -1) newIndices[i + 1] = newIndices[i + 4];
			if (newIndices[i + 3] == -1) newIndices[i + 3] = newIndices[i];
			if (newIndices[i + 5] == -1) newIndices[i + 5] = newIndices[i + 2];
		}

		adjacency = true;
		adjacencyIndices = newIndices;
	}

	void genUniqueIndexMap() {
		for (int i = 0; i < numFaces; i++) {
			int firstIdx = 3 * i; // The first index of the triangle in the index array

			// For each vertex position, check if in the map.
			// If not, add it and the current index. 
			for (int j = 0; j < 3; j++) {
				int idx = indices[firstIdx + j];
				glm::vec3 pos = vertexContainer.at<VertexImpl>(idx)->aPos;

				if (!posIndexMap.contains(pos)) {
					posIndexMap[pos] = idx;
				}
			}
		}
	}

	int findAdjacentVertexIdx(GLuint startIdx, GLuint endIdx, GLuint oppIdx) {
		for (int i = 0; i < numFaces; i++) {
			int firstIdx = 3 * i;

			// Get the unique indices for the vertices
			GLuint faceIndices[3];
			for (int j = 0; j < 3; j++) {
				GLuint idx = indices[firstIdx + j];
				faceIndices[j] = posIndexMap[vertexContainer.at<VertexImpl>(idx)->aPos];
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
