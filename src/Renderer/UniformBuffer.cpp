#include "vpch.h"
#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(Binder::UniformBufferInfo bufferInfo, GLenum hint) : binding(bufferInfo.binding), size(bufferInfo.size), drawHint(hint)
{}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : Buffer(std::move(other)), binding(other.binding), size(other.size), drawHint(other.drawHint) {
	other.buffer = 0;
}

