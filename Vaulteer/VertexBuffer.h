#pragma once

#include <vector>

#include "VertexArray.h"
#include "Buffer.h"

template<class Store>
class VertexBuffer : public Buffer<BufferType::ArrayBuffer>, public DebugLogger<VertexBuffer<void>> {
public:
	VertexBuffer();

	VertexBuffer(std::vector<Store>& vertices);

	VertexBuffer(size_t bufferSize, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	VertexBuffer(std::vector<Store>& vertices, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	//VertexBuffer(std::vector<Store>& vertices, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	VertexBuffer(VertexBuffer&& other) noexcept;


	void bindVertexArray(VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	void insert(const std::vector<Store>& vertices);

	void insertPartial(const size_t position, const std::vector<Store>& vertices);

	void reserve(const size_t bufferSize);

private:

};


template<class Store>
VertexBuffer<Store>::VertexBuffer() {}

template<class Store>
inline VertexBuffer<Store>::VertexBuffer(std::vector<Store>& vertices) {
	debug("VertexBuffer created. Buffer: " + std::to_string(buffer) + "\n");

	insert(vertices);
}

template<class Store>
inline VertexBuffer<Store>::VertexBuffer(size_t bufferSize, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors) {
	debug("VertexBuffer created with size " + std::to_string(bufferSize) + ".Buffer: " + std::to_string(buffer) + "\n");

	reserve(bufferSize);
	bindVertexArray(vertexArray, locInfo, divisors);
}

template<class Store>
inline VertexBuffer<Store>::VertexBuffer(std::vector<Store>& vertices, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors) {
	debug("VertexBuffer destroyed. Buffer: " + std::to_string(buffer) + "\n");

	insert(vertices);
	bindVertexArray(vertexArray, locInfo, divisors);
}


template<class Store>
inline VertexBuffer<Store>::VertexBuffer(VertexBuffer&& other) noexcept :
	Buffer(std::move(other)) {
	debug("VertexBuffer moved. Buffer: " + std::to_string(buffer) + "\n");
}


template<class Store>
void VertexBuffer<Store>::bindVertexArray(VertexArray& vao, LocationVector locInfo, LocationVector divisors) {
	//Make a class that can be inherited into a vertex class which then can be used to create vertex classes and the code below becomes more type safe.
	glVertexArrayVertexBuffer(vao, vao.bindIndex(), buffer, 0, sizeof(Store));
	vao.setUpAttributes(locInfo, divisors, sizeof(Store), typeid(Store).name());
}

template<class Store>
void VertexBuffer<Store>::insert(const std::vector<Store>& vertices) {
	size_t si = vertices.size() * sizeof(Store);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ARRAY_BUFFER, si, vertices.data(), GL_STATIC_DRAW);
	glNamedBufferData(buffer, vertices.size() * sizeof(Store), vertices.data(), GL_STATIC_DRAW);
	debug("Vertices inserted into buffer: " + std::to_string(buffer) + "\n");
}

template<class Store>
void VertexBuffer<Store>::insertPartial(const size_t position, const std::vector<Store>& vertices) {
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferSubData(GL_ARRAY_BUFFER, position * sizeof(Store), vertices.size() * sizeof(Store), vertices.data());
	glNamedBufferSubData(buffer, position * sizeof(Store), vertices.size() * sizeof(Store), vertices.data());
	debug("Vertex data replaced in buffer: " + std::to_string(buffer) + "\n");
}

template<class Store>
inline void VertexBuffer<Store>::reserve(const size_t bufferSize) {
	glNamedBufferStorage(buffer, bufferSize * sizeof(Store), nullptr, GL_DYNAMIC_STORAGE_BIT);
	//glNamedBufferData(buffer, bufferSize * sizeof(Store), nullptr, GL_DYNAMIC_DRAW);
}
