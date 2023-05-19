#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

enum class ShaderDataType {
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static int getShaderDataTypeSize(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
	}
	//assert(false); // "invalid type in vertex element"
	return 0;
}

static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
	}
	//assert(false); // "invalid type in vertex element"
	return 0;
}

struct VertexElement {

	VertexElement(std::string name, ShaderDataType type, int instance = 0, bool normalized = false)
		: name(std::move(name)), type(type), size(getShaderDataTypeSize(type)), offset(0), instance(instance), normalized(normalized) {
	}

	std::string name;
	ShaderDataType type;
	size_t size;
	size_t offset;
	int instance;
	bool normalized;

	int getComponentCount() const {
		switch (type) {
		case ShaderDataType::Float:    return 1;
		case ShaderDataType::Float2:   return 2;
		case ShaderDataType::Float3:   return 3;
		case ShaderDataType::Float4:   return 4;
		case ShaderDataType::Mat3:     return 3;
		case ShaderDataType::Mat4:     return 4;
		case ShaderDataType::Int:      return 1;
		case ShaderDataType::Int2:     return 2;
		case ShaderDataType::Int3:     return 3;
		case ShaderDataType::Int4:     return 4;
		case ShaderDataType::Bool:     return 1;
		default:                       
			//assert(false); // "invalid type in vertex element"
			return 0;
		}
	}

};

class BufferLayout {
public:
	BufferLayout() = default;

	BufferLayout(std::initializer_list<VertexElement> layout) : layout(layout) {
		updateOffsetsAndStride();
	}

	BufferLayout(BufferLayout& other) = default;

	const std::vector<VertexElement>& getElements() const {
		return layout;
	}
	
	size_t stride = 0;
private:
	void updateOffsetsAndStride() {
		size_t offset = 0;
		size_t strideTotal = 0;
		for (auto& element : layout) {
			element.offset = offset;
			offset += element.size;
			
			if (element.instance == 0) // instanced elements are part of their own VB and are skipped
				strideTotal += element.size;
		}
		this->stride = strideTotal;
	}

	// total size of layout
	std::vector<VertexElement> layout;
};
