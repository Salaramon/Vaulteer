#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/VertexArray.h"

typedef GLuint Index;

class ElementBuffer : public Buffer<BufferType::ElementBuffer> {
public:
	ElementBuffer();

	ElementBuffer(size_t bufferSize);

	ElementBuffer(size_t bufferSize, GLuint vao);

	ElementBuffer(std::vector<Index>& indices);

	ElementBuffer(std::vector<Index>& indices, GLuint vao);

	ElementBuffer(ElementBuffer&& other) noexcept;

	void insert(const std::vector<Index>& indices);

	void insertPartial(size_t position, const std::vector<Index>& indices);

	void reserve(size_t bufferSize);

	void bindVertexArray(GLuint vao);


	inline static auto CR = DY::ClassRegister<
		&insert,
		&insertPartial,
		&reserve,
		&bindVertexArray>(
			"insert",
			"insertPartial",
			"reserve",
			"bindVertexArray");

	inline static auto CB = DY::ClassBinder(CR, Buffer<BufferType::ElementBuffer>::CB);

	using LOG = _LOG<decltype(CB), decltype(Buffer<BufferType::ElementBuffer>::OB), DY::No_FB, DY::No_VB>;

};





