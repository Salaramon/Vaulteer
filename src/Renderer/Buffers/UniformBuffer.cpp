#include "vpch.h"
#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const Binder::UniformBufferInfo& bufferInfo, GLenum hint) : binding(bufferInfo.binding), size(bufferInfo.size), drawHint(hint) {
 	LOG::CTOR::debug<UniformBuffer>(this, std::format("Creating UBO for {} (size {})", bufferInfo.name, bufferInfo.size));
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : Buffer(std::move(other)), binding(other.binding), size(other.size), drawHint(other.drawHint) {
	other.buffer = 0;
}
