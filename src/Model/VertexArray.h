#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "Model/IndexBuffer.h"
#include "Model/VertexBuffer.h"
#include "Model/Storage/BufferLayout.h"
#include "Utils/TypeDefUtils.h"

class VertexArray {
public:
	operator GLuint() const { return vao; }

	VertexArray() {
		glCreateVertexArrays(1, &vao);
	}

	VertexArray(VertexArray& other) noexcept :
			vao(other.vao),
			setUp(other.setUp),
			vertexBuffers(std::move(other.vertexBuffers)),
			indexBuffer(std::move(other.indexBuffer)) {
		other.vao = 0;
	}

	VertexArray(VertexArray&& other) noexcept :
			vao(other.vao),
			setUp(other.setUp),
			vertexBuffers(std::move(other.vertexBuffers)),
			indexBuffer(std::move(other.indexBuffer)) {
		other.vao = 0;
	}

	~VertexArray() {
		glDeleteVertexArrays(1, &vao);
	}

	void bind() const {
		glBindVertexArray(vao);
	}

	static void unbind() {
		glBindVertexArray(0);
	}

	std::vector<VertexBuffer*> createVertexBuffers(const BufferLayout& bufferLayout) {
		setUpAttributes(bufferLayout);

		// create view
		std::vector<VertexBuffer*> buffers;
		for (auto& buf : vertexBuffers) {
			buffers.push_back(buf.get());
		}
		return buffers;
	}

	IndexBuffer* createIndexBuffer() {
		this->indexBuffer = std::make_unique<IndexBuffer>(vao);
		return indexBuffer.get();
	}

	IndexBuffer* createIndexBuffer(const std::vector<GLuint>& indices) {
		this->indexBuffer = std::make_unique<IndexBuffer>(indices, vao);
		return indexBuffer.get();
	}

	VertexBuffer* getVertexBuffer(int index) {
		return vertexBuffers.at(index).get();
	}

	IndexBuffer* getIndexBuffer() {
		return indexBuffer.get();
	}

private:
	void setUpAttributes(const BufferLayout& format);
	
	GLuint vao;

	bool setUp = false;
	std::vector<std::unique_ptr<VertexBuffer>> vertexBuffers;
	std::unique_ptr<IndexBuffer> indexBuffer;
};

inline void VertexArray::setUpAttributes(const BufferLayout& format) {
	if (setUp)
		return;

	vertexBuffers.push_back(std::make_unique<VertexBuffer>(vao, format.stride));
	//std::cout << "- created initial vertexbuffer: stride " << format.stride << std::endl;

	int location = 0;

	for (const VertexElement& element : format.getElements()) {
		uint offset;
		uint bindIndex = 0;
		uint comp = 0;

		if (element.instance > 0) {
			bindIndex = vertexBuffers.size();
			vertexBuffers.push_back(std::make_unique<VertexBuffer>(vao, element.size, bindIndex));
			//std::cout << "- created sub-vertexbuffer " << bindIndex << ": stride " << element.size << std::endl;
			offset = 0;
		} else {
			offset = element.offset;
		}

		glEnableVertexArrayAttrib(vao, location);

		switch (element.type) {
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			glVertexArrayAttribIFormat(vao,
				location,
				element.getComponentCount(),
				shaderDataTypeToOpenGLBaseType(element.type),
				offset);
			break;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			glVertexArrayAttribFormat(vao, 
				location,
				element.getComponentCount(),
				shaderDataTypeToOpenGLBaseType(element.type),
				element.normalized,
				offset);
			break;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		{
			while (comp < element.getComponentCount()) {
				uint8_t compSize = element.size / element.getComponentCount();

				glEnableVertexArrayAttrib(vao, location + comp);
				glVertexArrayAttribFormat(vao, 
					location + comp,
					element.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(element.type),
					element.normalized,
					offset + comp * compSize
				);
				glVertexArrayAttribBinding(vao, location + comp, bindIndex);

				comp++;
			}
			location += comp;
			break;
		}
		case ShaderDataType::Bool:
		default:
			assert(false); // implement as needed
			break;
		}

		glVertexArrayAttribBinding(vao, location, bindIndex);

		if (element.instance > 0)
			glVertexArrayBindingDivisor(vao, bindIndex, element.instance);

		if (!comp)
			location++;
	}

	setUp = true;
}
