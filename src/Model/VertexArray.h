#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "Model/IndexBuffer.h"
#include "Model/VertexBuffer.h"
#include "Model/Storage/BufferLayout.h"

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

	VertexBuffer* createVertexBuffer(const BufferLayout& bufferLayout) {
		vertexBuffers.push_back(std::make_unique<VertexBuffer>(vao, bufferLayout));
		setUpAttributes(bufferLayout);
		return vertexBuffers.back().get();
	}

	VertexBuffer* createVertexBuffer(const BufferLayout& bufferLayout, const void* data, size_t count) {
		VertexBuffer* vb = createVertexBuffer(bufferLayout);
		vb->insert(data, count);
		return vb;
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

	/* Binding index for VBO -> binding -> VAO API.
	 * We don't need anything but 0 (it's only used when referring to multiple buffers with one format) */
	GLuint bindIndex() const {
		return 0;
	}
};

inline void VertexArray::setUpAttributes(const BufferLayout& format) {
	if (setUp)
		return;

	// max size of single vertex attribute as defined by opengl - elements exceeding this size must be split up into more locations 
	constexpr size_t maxSize = 16;

	int location = 0;
	for (const VertexElement& element : format.getElements()) {
		
		std::cout << "current location " << location << std::endl;
		std::cout << "element name " << element.name << std::endl;
		std::cout << "element size " << element.size << std::endl;

		if (element.size > maxSize) {

			int componentSize = element.size / element.getComponentCount();

			std::cout << "component size " << componentSize << std::endl;

			int comp = 0;
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
			location += comp;
		}
		else {
			glEnableVertexArrayAttrib(vao, location);

			if (element.type == ShaderDataType::Int) {
				glVertexArrayAttribIFormat(vao,
					location,
					element.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(element.type),
					element.offset);
			}
			else {
				glVertexArrayAttribFormat(vao, 
					location,
					element.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(element.type),
					element.normalized,
					element.offset);
			}


			glVertexArrayAttribBinding(vao, location, bindIndex());	

			if (element.instance > 0)
				glVertexArrayBindingDivisor(vao, bindIndex(), element.instance);
			
			location++;
		}
	}

	setUp = true;
}
