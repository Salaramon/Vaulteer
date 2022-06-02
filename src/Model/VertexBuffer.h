#pragma once

#include <vector>

#include "Model/VertexArray.h"
#include "Model/Buffer.h"

template<class Store>
class VertexBuffer : public Buffer<BufferType::ArrayBuffer>, public DebugLogger<VertexBuffer<void>> {
public:
	// init single vertexarray for each Store used with VertexBuffer
	//VertexArray<Store>* vao = new VertexArray<Store>();
	static VertexArray<Store>& getVAO() {
		static VertexArray<Store> vao;
		return vao;
	}

	operator GLuint() const { return buffer; }

	VertexBuffer();

	//VertexBuffer(std::vector<Store>& vertices);

	VertexBuffer(size_t bufferSize);

	VertexBuffer(std::vector<Store>& vertices);

	//VertexBuffer(std::vector<Store>& vertices, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	VertexBuffer(VertexBuffer& other) noexcept = delete;

	VertexBuffer(VertexBuffer&& other) noexcept;


	void insert(const std::vector<Store>& vertices);

	void insertPartial(const size_t position, const std::vector<Store>& vertices);

	void reserve(const size_t bufferSize);

//private:
	void bindVertexArray();

	GLuint getBuffer() { return buffer; }
};


template<class Store>
VertexBuffer<Store>::VertexBuffer() {}

template<class Store>
VertexBuffer<Store>::VertexBuffer(size_t bufferSize) {
	debug("VertexBuffer created with size " + std::to_string(bufferSize) + ".Buffer: " + std::to_string(buffer) + "\n");

	reserve(bufferSize);
	bindVertexArray();

	VertexArray<Store>& vao = getVAO();
	vao.setUpAttributes(Store::locInfo, Store::locDivisors, sizeof(Store), typeid(Store).name());
}

template<class Store>
VertexBuffer<Store>::VertexBuffer(std::vector<Store>& vertices) {
	debug("VertexBuffer destroyed. Buffer: " + std::to_string(buffer) + "\n");

	insert(vertices);
	bindVertexArray();

	VertexArray<Store>& vao = getVAO();
	vao.setUpAttributes(Store::locInfo, Store::locDivisors, sizeof(Store), typeid(Store).name());
}


template<class Store>
VertexBuffer<Store>::VertexBuffer(VertexBuffer&& other) noexcept :
	Buffer(std::move(other)) {
	//other.vao = nullptr;
	debug("VertexBuffer moved. Buffer: " + std::to_string(buffer) + "\n");
}


template<class Store>
void VertexBuffer<Store>::bindVertexArray() {
	VertexArray<Store>& vao = getVAO();
	glVertexArrayVertexBuffer(vao, vao.bindIndex(), buffer, 0, sizeof(Store));
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
