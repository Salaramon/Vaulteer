#pragma once

#include <typeindex>
#include <glad/glad.h>

#include "Model/VertexBuffer.h"
#include "Model/IndexBuffer.h"
#include "Model/VertexArray.h"

#include "Model/Material.h"

#include "Debug/Debug.h"


class VertexContainer {
public:
	template<vertex_concept T>
	VertexContainer(T t) : vertexType(typeid(T)), format(T::getFormat()) {}

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
        return format.getStride();
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


class Mesh {
public:
	template<vertex_concept T>
	Mesh(std::vector<T> vertices, std::vector<GLuint>& indices, Material* material) :
		vertexContainer(vertices),
		indices(indices),
		material(material),

		OR(this,
		   DY::V(&this->vertexContainer, &this->indices, &this->vertexArray, &this->material),
		   DY::N("vertices", "indices", "vertexBuffer", "indexBuffer", "material")) {

		vertexBuffer = vertexArray.createVertexBuffer(T::getFormat(), vertexContainer.data(), vertexContainer.size());
		indexBuffer = vertexArray.createIndexBuffer(this->indices);

		LOG::CTOR::debug(this, "Mesh was created");
	}

	Mesh(Mesh& other) = delete;

	Mesh(Mesh&& other) noexcept : 
		vertexContainer(std::move(other.vertexContainer)),
		indices(std::move(other.indices)),
		material(other.material),
		vertexArray(std::move(other.vertexArray)),
		vertexBuffer(other.vertexBuffer),
		indexBuffer(other.indexBuffer) {

		LOG::CTOR::debug(this, "Mesh was moved");
	}

	~Mesh() {
		LOG::CTOR::debug(this, "Mesh was destroyed");
	}

	void bind() {
		vertexArray.bind();
		vertexBuffer->bindVertexArray(vertexArray);
		indexBuffer->bindVertexArray(vertexArray);
		LOG::CLAS::debug<&Mesh::bind>(this, "Mesh was bound.");
	}
	void unbind() {
		vertexArray.unbind();
		LOG::CLAS::debug<&Mesh::unbind>(this, "Mesh was unbound.");
	}

	void updateBuffer() {
		vertexBuffer->insert(vertexContainer.data(), vertexContainer.size());
	}

	template<vertex_concept T>
	std::vector<T> getCopiedData() {
		T* arr = (T*) vertexContainer.data();
		return std::vector<T>(arr, arr + vertexContainer.size());
	}

	VertexContainer vertexContainer;
	std::vector<GLuint> indices;
	Material* material;

	VertexArray vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	inline static auto CR = DY::ClassRegister<
		&bind,
		&unbind>();
	inline static auto CB = DY::ClassBinder<decltype(CR)>();

	DY::ObjectRegister<Mesh,
		decltype(vertexContainer),
		decltype(indices),
		decltype(vertexArray),
		decltype(material)> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};


