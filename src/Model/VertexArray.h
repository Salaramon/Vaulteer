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

	void unbind() const {
		glBindVertexArray(0);
	}

	std::vector<VertexBuffer*> createVertexBuffers(const BufferLayout& bufferLayout) {
		setUpAttributes(bufferLayout);

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
		IndexBuffer* ib = createIndexBuffer();
		ib->insert(indices);
		return ib;
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
	std::cout << "- created initial vertexbuffer: stride " << format.stride << std::endl;

	int mainLocation = 0;
	int location = 0;

	for (const VertexElement& element : format.getElements()) {
		uint offset;
		uint bindIndex = 0;

		if (element.instance > 0) {
			bindIndex = vertexBuffers.size();
			vertexBuffers.push_back(std::make_unique<VertexBuffer>(vao, element.size, bindIndex));
			std::cout << "- created sub-vertexbuffer " << bindIndex << ": stride " << element.size << std::endl;
			offset = 0;
		} else {
			offset = element.offset;
		}
		
		std::cout << "current location " << location << std::endl;
		std::cout << "element name " << element.name << std::endl;
		std::cout << "element size " << element.size << std::endl;
		std::cout << "element offset " << offset << std::endl;
		std::cout << "element bind " << bindIndex << std::endl;

		glEnableVertexArrayAttrib(vao, location);

		std::cout << "element comp count " << element.getComponentCount() << std::endl;

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
			
			// TODO : implement split again
			/*int comp = 0;
			while (comp < element.getComponentCount()) {
				std::cout << "  component loc " << location + comp << std::endl;
				std::cout << "  component offset " << location + comp * componentSize << std::endl;

				glEnableVertexArrayAttrib(vao, location + comp * componentSize);
				glVertexArrayAttribFormat(vao, 
					location + comp,
					componentSize,
					shaderDataTypeToOpenGLBaseType(element.type),
					element.normalized,
					element.offset + comp * componentSize
				);
				glVertexArrayAttribBinding(vao, location, bindIndex());

				if (element.instance > 0)
					glVertexArrayBindingDivisor(vao, bindIndex(), element.instance);
				
				std::cout << "  set comp " << comp << std::endl;
				comp++;
			}
			location += comp;*/
		case ShaderDataType::Bool:
		default:
			assert(false); // implement as needed
			break;
		}

		glVertexArrayAttribBinding(vao, location, bindIndex);

		if (element.instance > 0)
			glVertexArrayBindingDivisor(vao, bindIndex, element.instance);
		
		location++;
	}

	setUp = true;
}
