#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(Binder::UniformBufferInfo bufferInfo, DrawHint hint) : binding(bufferInfo.binding), size(bufferInfo.size), drawHint(hint) {
	glCreateBuffers(1, &UBO);
}

UniformBuffer::UniformBuffer(UniformBuffer& other) noexcept : UBO(other.UBO), binding(other.binding), size(other.size), drawHint(other.drawHint) {
	other.UBO = 0;
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : UBO(other.UBO), binding(other.binding), size(other.size), drawHint(other.drawHint) {
	other.UBO = 0;
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &UBO);
}

