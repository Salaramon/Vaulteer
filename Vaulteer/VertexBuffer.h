#pragma once

#include <vector>

#include "VertexArray.h"
#include "Buffer.h"

typedef std::vector<Binder::LocationInfo> LocationVector;

template<class Store>
class VertexBuffer : public Buffer<BufferType::ArrayBuffer>, public DebugLogger<VertexBuffer<void>> {
public:
	VertexBuffer();

	VertexBuffer(std::vector<Store>& vertices);

	VertexBuffer(size_t bufferSize);

	VertexBuffer(std::vector<Store>& vertices, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	//VertexBuffer(std::vector<Store>& vertices, VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	VertexBuffer(VertexBuffer&& other) noexcept;


	void bindVertexArray(VertexArray& vertexArray, LocationVector locInfo, LocationVector divisors = LocationVector(0));

	void setVertexDivisors(VertexArray& vertexArray, size_t location, size_t subLoc, LocationVector locDivs);

	void insert(const std::vector<Store>& vertices);

	void insertPartial(const size_t position, const std::vector<Store>& vertices);

private:
	void bindDebugMessage(size_t location, std::string name, size_t size, size_t stride, size_t offset, std::string structName);

};


template<class Store>
VertexBuffer<Store>::VertexBuffer() {}

template<class Store>
inline VertexBuffer<Store>::VertexBuffer(size_t bufferSize) {

}


template<class Store>
inline VertexBuffer<Store>::VertexBuffer(std::vector<Store>& vertices) {
	debug("VertexBuffer created. Buffer: " + std::to_string(buffer) + "\n");

	insert(vertices);
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
	const size_t maxSize = 16;
	size_t offset = 0;
	for (size_t i = 0; i < locInfo.size(); i++) {
		if (locInfo[i].size > maxSize) {

			// TODO - this is an untested DSA implementation

			size_t locNr = locInfo[i].size / maxSize;
			for (size_t j = 0; j < locNr; j++) {
				glEnableVertexArrayAttrib(vao, locInfo[i].loc + j);

				bindDebugMessage(locInfo[i].loc + j, locInfo[i].name, locInfo[i].size / maxSize, sizeof(Store), (offset + (j * maxSize)), typeid(Store).name());
				//glVertexAttribPointer(locInfo[i].loc + j, locInfo[i].size / maxSize, GL_FLOAT, GL_FALSE, sizeof(Store), (void*)(offset + (j * maxSize)));
				glVertexArrayAttribFormat(vao, locInfo[i].loc, locInfo[i].size / maxSize, GL_FLOAT, GL_FALSE, offset + (j * maxSize));
				glVertexArrayAttribBinding(vao, locInfo[i].loc + j, vao.bindIndex());
				setVertexDivisors(vao, locInfo[i].loc, j, divisors);
			}
		}
		else {
			bindDebugMessage(locInfo[i].loc, locInfo[i].name, locInfo[i].size / sizeof(float), sizeof(Store), offset, typeid(Store).name());
			glEnableVertexArrayAttrib(vao, locInfo[i].loc);
			//glVertexAttribPointer(locInfo[i].loc, locInfo[i].size / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Store), (void*)offset);
			glVertexArrayAttribFormat(vao, locInfo[i].loc, locInfo[i].size / sizeof(float), GL_FLOAT, GL_FALSE, offset);
			glVertexArrayAttribBinding(vao, locInfo[i].loc, vao.bindIndex());
			setVertexDivisors(vao, locInfo[i].loc, 0, divisors);
		}
		offset += locInfo[i].size;
	}
	//Make ShaderBinder able to return the size of the vertex struct currently in use.
	//MAYBE: Make Shader class take a parameter which defines what structure it should use.
	//Make VertexAttribute take a template argument taking an attribute object type, which will be what it generates attributes for.
}

template<class Store>
inline void VertexBuffer<Store>::setVertexDivisors(VertexArray& vao, size_t location, size_t subLoc, LocationVector locDivs) {

	for (size_t i = 0; i < locDivs.size(); i++) {
		if (locDivs[i].loc == location) {
			glVertexArrayBindingDivisor(vao, vao.bindIndex(), 1);
			//glVertexAttribDivisor(location + subLoc, 1);
			debug("Attribute divisor set for location: " + std::to_string(location + subLoc) + "\n", "glVertexAttribDivisor");
		}
	}
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
inline void VertexBuffer<Store>::bindDebugMessage(size_t location, std::string name, size_t size, size_t stride, size_t offset, std::string structName) {
	debug("Vertex attribute pointer set with:\n\tlocation: " + std::to_string(location) + "\n\tsize: " + std::to_string(size) + "\n\tstride: " + std::to_string(stride) + "\n\toffset: " + std::to_string(offset) + "\n\tstruct: " + structName + "\n", "glVertexAttribPointer");
}