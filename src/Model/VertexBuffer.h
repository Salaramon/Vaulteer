#pragma once

#include <vector>

#include "Model/VertexArray.h"
#include "Model/Buffer.h"

template<class Store>
class VertexBuffer : public Buffer<BufferType::ArrayBuffer> {
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

	inline static auto CR = DY::ClassRegister<
		&insert,
		&insertPartial,
		&reserve,
		&bindVertexArray,
		&getBuffer>(
			"insert",
			"insertPartial",
			"reserve",
			"bindVertexArray"
			"getBuffer");

	inline static auto FR = DY::FunctionRegister<&getVAO>("getVAO");

	//Uncomment when ClassBinders and FunctionBinders for Buffer and VertexArray is added.
	inline static auto CB = DY::ClassBinder(CR/*, Buffer<BufferType::ArrayBuffer>::CB, VertexBuffer<Store>::CB*/);
	inline static auto FB = DY::FunctionBinder(FR/*, Buffer<BufferType::VertexArray>::FB, VertexArray<Store>::FB*/);


	using LOG = _LOG<decltype(CB), DY::No_OB, decltype(FB), DY::No_VB>;
};


template<class Store>
VertexBuffer<Store>::VertexBuffer() {
	LOG::CTOR(this, "Empty VertexBuffer created");
}

template<class Store>
VertexBuffer<Store>::VertexBuffer(size_t bufferSize) {
	//debug("VertexBuffer created with size " + std::to_string(bufferSize) + ".Buffer: " + std::to_string(buffer) + "\n");
	LOG::CTOR::debug(this, std::format("Creating VertexBuffer of size {}", bufferSize));

	reserve(bufferSize);
	bindVertexArray();

	VertexArray<Store>& vao = getVAO();
	vao.setUpAttributes(Store::locInfo, Store::locDivisors, sizeof(Store), typeid(Store).name());
}

template<class Store>
VertexBuffer<Store>::VertexBuffer(std::vector<Store>& vertices) {
	LOG::CTOR::debug(this, "Creating VertexBuffer from vertices...");

	insert(vertices);
	bindVertexArray();

	VertexArray<Store>& vao = getVAO();
	vao.setUpAttributes(Store::locInfo, Store::locDivisors, sizeof(Store), typeid(Store).name());
}


template<class Store>
VertexBuffer<Store>::VertexBuffer(VertexBuffer&& other) noexcept :
	Buffer(std::move(other)) {
	//other.vao = nullptr;

	LOG::CTOR::debug(this, &other, "Vertex buffer was moved");
}


template<class Store>
void VertexBuffer<Store>::bindVertexArray() {
	VertexArray<Store>& vao = getVAO();
	glVertexArrayVertexBuffer(vao, vao.bindIndex(), buffer, 0, sizeof(Store));

	LOG::CLAS::debug<&VertexBuffer<Store>::bindVertexArray>(this, &vao, std::format("Vertex array of id {} was bound to buffer with id {}", static_cast<GLuint>(vao), buffer));
}

template<class Store>
void VertexBuffer<Store>::insert(const std::vector<Store>& vertices) {
	size_t si = vertices.size() * sizeof(Store);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ARRAY_BUFFER, si, vertices.data(), GL_STATIC_DRAW);
	glNamedBufferData(buffer, vertices.size() * sizeof(Store), vertices.data(), GL_STATIC_DRAW);

	LOG::CLAS::debug<&VertexBuffer<Store>::insert>(this, &buffer, std::format("Vertices inserted into buffer: {}", buffer));
}

template<class Store>
void VertexBuffer<Store>::insertPartial(const size_t position, const std::vector<Store>& vertices) {
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferSubData(GL_ARRAY_BUFFER, position * sizeof(Store), vertices.size() * sizeof(Store), vertices.data());
	glNamedBufferSubData(buffer, position * sizeof(Store), vertices.size() * sizeof(Store), vertices.data());
	
	LOG::CLAS::debug<&VertexBuffer<Store>::insertPartial>(this, &buffer, std::format("Vertex data replaced in buffer: {}", buffer));
}

template<class Store>
inline void VertexBuffer<Store>::reserve(const size_t bufferSize) {
	glNamedBufferStorage(buffer, bufferSize * sizeof(Store), nullptr, GL_DYNAMIC_STORAGE_BIT);
	//glNamedBufferData(buffer, bufferSize * sizeof(Store), nullptr, GL_DYNAMIC_DRAW);
}
