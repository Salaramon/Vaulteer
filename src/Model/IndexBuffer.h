#pragma once

#include <vector>

#include "Model/Buffer.h"

class IndexBuffer : public ElementArrayBuffer {
public:
	IndexBuffer();

	IndexBuffer(GLuint vao);

	IndexBuffer(const std::vector<GLuint>& indices);

	IndexBuffer(const std::vector<GLuint>& indices, GLuint vao);

	IndexBuffer(IndexBuffer& other) noexcept;

	IndexBuffer(IndexBuffer&& other) noexcept;

	void insert(const std::vector<GLuint>& indices);

	void insertPartial(size_t position, const std::vector<GLuint>& indices);

	void reserve(size_t bufferSize) const;

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

	/*inline static auto CB = DY::ClassBinder(CR, ElementArrayBuffer::CB);

	using LOG = _LOG<decltype(CB), decltype(ElementArrayBuffer::OB), DY::No_FB, DY::No_VB>;*/

};
