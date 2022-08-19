#pragma once

#include "Renderer/Shader.h"

class GraphicsData {
	//virtual void draw(const Shader& shader) = 0;

protected:
	template <class Hash, class ...Args>
	std::vector<typename Hash::VertexType>& storePointsAndIndices(std::vector<GLuint>& indices, std::vector<typename Hash::VertexType>& vertices, glm::vec3 first, Args ... args) {
		std::vector<typename Hash::VertexType> allVertices({args...});
		std::unordered_map<typename Hash::VertexType, GLuint, Hash, Hash> filter;
		for (size_t i = 0; i < allVertices.size(); i++) {
			auto firstInsert = filter.emplace(allVertices[i], vertices.size());
			if (firstInsert.second) {
				vertices.push_back(firstInsert.first->first);
				indices.push_back(firstInsert.first->second);
			}
			else {
				indices.push_back(firstInsert.first->second);
			}
		}

		return vertices;
	}


	template <class Hash>
	std::vector<typename Hash::VertexType>& storePointsAndIndices(std::vector<GLuint>& indices, std::vector<typename Hash::VertexType>& vertices, std::vector<typename Hash::VertexType> insertedVertices) {
		std::vector<typename Hash::VertexType> allVertices(insertedVertices);
		std::unordered_map<typename Hash::VertexType, GLuint, Hash, Hash> filter;
		for (size_t i = 0; i < allVertices.size(); i++) {
			auto firstInsert = filter.emplace(allVertices[i], vertices.size());
			if (firstInsert.second) {
				vertices.push_back(firstInsert.first->first);
				indices.push_back(firstInsert.first->second);
			}
			else {
				indices.push_back(firstInsert.first->second);
			}
		}

		return vertices;
	}

};
