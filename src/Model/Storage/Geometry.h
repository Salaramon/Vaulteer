#pragma once
#include "VertexContainer.h"

struct Geometry {
	VertexContainer vertices; 
	std::vector<GLuint> indices;
	GLenum type = GL_TRIANGLES;
	
	template <class T>
	Geometry(std::vector<T> vertices, std::vector<GLuint> indices, GLenum type) :
			vertices(vertices), indices(indices), type(type) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot create geometry from non-vertices.");
	}

	bool hasAdjacency() {
		return type == GL_TRIANGLES_ADJACENCY;
	}
	

	template <class T>
	std::vector<T> copyVertices() {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot copy out vertices to non-vertex type.");
		T* arr = (T*) vertices.data();
		return std::vector<T>(arr, arr + vertices.size());
	}

	template <class T>
	T* getVertex(int index) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot copy out vertices to non-vertex type.");
		return vertices.at<T>(index);
	}
};
