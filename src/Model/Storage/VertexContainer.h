#pragma once

#include <typeindex>

#include "Vertex.h"
#include "BufferLayout.h"

// Generic container for vertex types to be referenced directly in vertexbuffer insertion
class VertexContainer {
public:
	template<vertex_concept T>
	VertexContainer(T) : vertexType(typeid(T)), format(T::getFormat()) {}

	template<vertex_concept T>
	VertexContainer(std::vector<T> vertices) : vertexType(typeid(T)), format(T::getFormat()) {
		for (T vertex : vertices)
			this->add<T>(vertex);
	}

	template<vertex_concept T>
	void add(T& t) {
		std::array<uint8_t, sizeof(T)> bytes{};
		auto beginObject = reinterpret_cast<uint8_t*>(std::addressof(t));
		memcpy_s(bytes.data(), bytes.size(), beginObject, bytes.size());

		vertexData.insert(vertexData.end(), std::begin(bytes), std::end(bytes));
		vertexCount++;
	}

	template<vertex_concept T>
	T* at(size_t index) {
		assert(index < vertexCount);
		return reinterpret_cast<T*>(vertexData.data() + index * stride());
	}
	
	template<vertex_concept T>
	void set(size_t index, T& t) {
		assert(index < vertexCount);
		T* ptr = reinterpret_cast<T*>(vertexData.data() + index * stride());
		memcpy_s(ptr, stride(), &t, stride());
	}

	
    const BufferLayout& getFormat() const {
        return format;
    }
	
    size_t size() const {
        return vertexCount;
    }

    size_t stride() const {
        return format.stride;
    }
	
    void* data() {
        return vertexData.data();
    }

private:
	std::vector<uint8_t> vertexData;
	size_t vertexCount = 0;

	std::type_index vertexType;
	BufferLayout& format;
};
